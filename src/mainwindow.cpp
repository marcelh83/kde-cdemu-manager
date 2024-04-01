/****************************************************************************
 *                                                                          *
 *   This file is part of KDE CDEmu Manager.                                *
 *                                                                          *
 *   Copyright (C) 2009-2024 by Marcel Hasler <mahasler@gmail.com>          *
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                          *
 ****************************************************************************/

#include "devicelistitem.h"
#include "mainwindow.h"
#include "messagebox.h"

#include "ui_mainwindow.h"

#include <KStandardAction>

#include <QFileDialog>
#include <QHeaderView>

// ---------------------------------------------------------------------------------------------- //

namespace {
    constexpr const char* FileTypes = "Images (*.mds *.mdx *.b5t *.b6t *.ccd *.sub *.img *.cue "
                                              "*.bin *.toc *.cdi *.cif *.c2d *.iso *.nrg *.udf);;"
                                      "Containers (*.dmg *.cdr *.cso *.ecm *.gz "
                                                  "*.gbi *.daa *.isz *.xz)";
    constexpr int MaxHistorySize = 10;

    constexpr const char* HistoryKey = "history";
    constexpr const char* ShowTrayIconKey = "showTrayIcon";
    constexpr const char* LastFilePathKey = "lastFilePath";
}

// ---------------------------------------------------------------------------------------------- //

MainWindow::MainWindow(const CDEmu& cdemu, QWidget* parent)
    : KMainWindow(parent),
      m_ui(std::make_unique<Ui::MainWindow>()),
      m_cdemu(cdemu)
{
    // Keep running in system tray if enabled
    setAttribute(Qt::WA_DeleteOnClose, false);

    m_ui->setupUi(this);

    // Menus
    m_ui->menuFile->addAction(KStandardAction::quit(qApp, SLOT(quit()), this));

    updateHistory();

    m_helpMenu = new KHelpMenu(this);
    menuBar()->addMenu(m_helpMenu->menu());

    // Tray icon
    QSettings settings;

    const bool showTrayIcon = settings.value(ShowTrayIconKey, true).toBool();
    setTrayIconVisible(showTrayIcon);

    m_ui->actionTrayIcon->setChecked(showTrayIcon);
    connect(m_ui->actionTrayIcon, SIGNAL(toggled(bool)), this, SLOT(setTrayIconVisible(bool)));

    // Device list
    m_ui->deviceList->header()->setStretchLastSection(false);

    m_ui->deviceList->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_ui->deviceList->header()->setSectionResizeMode(1, QHeaderView::Stretch);

    const QString header = i18n(m_ui->deviceList->headerItem()->text(0).toLocal8Bit()) + "xxx";
    m_ui->deviceList->header()->resizeSection(0, QFontMetrics(font()).horizontalAdvance(header));

    // Device handling
    connect(m_ui->addDevice, SIGNAL(clicked()), this, SLOT(addDevice()));
    connect(m_ui->removeDevice, SIGNAL(clicked()), this, SLOT(removeDevice()));

    connect(&m_cdemu, SIGNAL(deviceAdded()), this, SLOT(updateDeviceList()));
    connect(&m_cdemu, SIGNAL(deviceRemoved()), this, SLOT(updateDeviceList()));
    connect(&m_cdemu, SIGNAL(deviceChanged(int)), this, SLOT(onDeviceChanged(int)));
    connect(&m_cdemu, SIGNAL(daemonChanged(bool)), this, SLOT(onDaemonChanged(bool)));

    // Status bar
    m_statusLabel = new QLabel(this);
    m_statusLabel->setIndent(10);
    statusBar()->addWidget(m_statusLabel);
    onDaemonChanged(m_cdemu.isDaemonRunning());

    // Remember window size, etc.
    setAutoSaveSettings();
}

// ---------------------------------------------------------------------------------------------- //

MainWindow::~MainWindow() = default;

// ---------------------------------------------------------------------------------------------- //

void MainWindow::closeEvent(QCloseEvent* event)
{
    KMainWindow::closeEvent(event);

    if (!m_trayIcon)
        qApp->quit();
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::onDaemonChanged(bool running)
{
    Q_ASSERT(m_statusLabel != nullptr);

    m_ui->centralWidget->setEnabled(running);

    if (running)
    {
        updateDeviceList();
        m_statusLabel->setText(i18n("CDEmu daemon is running."));
    }
    else
        m_statusLabel->setText(i18n("CDEmu daemon not running."));
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::onDeviceChanged(int index)
{
    // CDEmu emits "DeviceStatusChanged" before "DeviceRemoved" if device was loaded
    auto item = dynamic_cast<DeviceListItem*>(m_ui->deviceList->topLevelItem(index));

    if (index < m_cdemu.getDeviceCount() && item)
        item->setFileName(m_cdemu.getFileName(index));
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::updateDeviceList()
{
    m_ui->deviceList->clear();

    const int deviceCount = m_cdemu.getDeviceCount();

    for (int i = 0; i < deviceCount; ++i)
    {
        auto item = new DeviceListItem(i);
        item->setFileName(m_cdemu.getFileName(i));

        connect(item, SIGNAL(mountClicked(int)), this, SLOT(mount(int)));
        connect(item, SIGNAL(unmountClicked(int)), this, SLOT(unmount(int)));

        m_ui->deviceList->addTopLevelItem(item);
        m_ui->deviceList->setItemWidget(item, 0, new QLabel(QString("  %1").arg(i)));
        m_ui->deviceList->setItemWidget(item, 1, item->widget());
    }

    m_ui->removeDevice->setEnabled(m_ui->deviceList->topLevelItemCount() > 0);
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::mount(int index)
{
    QSettings settings;
    QString path = settings.value(LastFilePathKey, QDir::homePath()).toString();

    const QString filename =
            QFileDialog::getOpenFileName(this, i18n("Select an image file"), path, FileTypes);

    if (filename.isEmpty())
        return;

    path = QFileInfo(filename).path();
    settings.setValue(LastFilePathKey, path);

    try {
        m_cdemu.mount(filename, index);
        appendHistory(filename);
    }
    catch (const Exception& e) {
        MessageBox::error(e.what());
    }
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::unmount(int index)
{
    try {
        m_cdemu.unmount(index);
    }
    catch (const Exception& e) {
        MessageBox::error(e.what());
    }
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::mountFromHistory()
{
    const auto action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action != nullptr);

    try {
        const QString filename = action->data().toString();
        const int index = m_cdemu.getNextFreeDevice();

        m_cdemu.mount(filename, index);
        appendHistory(filename);
    }
    catch (const Exception& e) {
        MessageBox::error(e.what());
    }
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::clearHistory()
{
    QSettings settings;
    settings.setValue(HistoryKey, QStringList());

    updateHistory();
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::addDevice()
{
    try {
        m_cdemu.addDevice();
    }
    catch (const Exception& e) {
        MessageBox::error(e.what());
    }
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::removeDevice()
{
    try {
        m_cdemu.removeDevice();
    }
    catch (const Exception& e) {
        MessageBox::error(e.what());
    }
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::setTrayIconVisible(bool visible)
{
    Q_ASSERT(m_helpMenu != nullptr);

    if (visible)
    {
        if (!m_trayIcon)
        {
            m_trayIcon = new KStatusNotifierItem(this);
            m_trayIcon->setIconByName("media-optical");
            m_trayIcon->setCategory(KStatusNotifierItem::ApplicationStatus);
            m_trayIcon->setStatus(KStatusNotifierItem::Active);
            m_trayIcon->setToolTip("media-optical", "KDE CDEmu Manager", "");
            m_trayIcon->contextMenu()->addMenu(m_helpMenu->menu());
        }
    }
    else
    {
        delete m_trayIcon;
        m_trayIcon = nullptr;
    }

    QSettings settings;
    settings.setValue(ShowTrayIconKey, visible);
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::appendHistory(const QString& filename)
{
    QSettings settings;
    QStringList history = settings.value(HistoryKey).toStringList();

    history.removeAll(filename);
    history.prepend(filename);

    settings.setValue(HistoryKey, history);

    updateHistory();
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::updateHistory()
{
    QSettings settings;
    QStringList history = settings.value(HistoryKey).toStringList();

    // Delete invalid entries
    QStringList::iterator it = history.begin();

    while (it != history.end())
    {
        if (!QDir().exists(*it))
            it = history.erase(it);
        else
            ++it;
    }

    // Limit size
    while (history.size() > MaxHistorySize)
        history.removeLast();

    settings.setValue(HistoryKey, history);

    // Rebuild menu
    m_ui->menuHistory->clear();

    for (int i = 0; i < history.size(); ++i)
    {
        QAction* action = m_ui->menuHistory->addAction(history.at(i));
        action->setData(history.at(i));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(mountFromHistory()));
    }

    m_ui->menuHistory->addSeparator();

    QAction* action = m_ui->menuHistory->addAction(i18n("Clear History"));
    action->setIcon(QIcon::fromTheme("edit-clear-history"));
    action->setEnabled(!history.isEmpty());
    connect(action, SIGNAL(triggered(bool)), this, SLOT(clearHistory()));
}

// ---------------------------------------------------------------------------------------------- //
