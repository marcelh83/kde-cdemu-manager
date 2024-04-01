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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cdemu.h"

#include <KHelpMenu>
#include <KMainWindow>
#include <KStatusNotifierItem>

#include <QLabel>

#include <memory>

namespace Ui {
    class MainWindow;
}

class MainWindow : public KMainWindow
{
    Q_OBJECT

public:
    MainWindow(const CDEmu& cdemu, QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onDaemonChanged(bool);
    void onDeviceChanged(int index);

    void updateDeviceList();

    void mount(int index);
    void unmount(int index);

    void mountFromHistory();
    void clearHistory();

    void addDevice();
    void removeDevice();

    void setTrayIconVisible(bool visible);

private:
    void closeEvent(QCloseEvent* event) override;

    void appendHistory(const QString& filename);
    void updateHistory();

private:
    std::unique_ptr<Ui::MainWindow> m_ui;

    const CDEmu& m_cdemu;

    QLabel* m_statusLabel = nullptr;

    KHelpMenu* m_helpMenu = nullptr;
    KStatusNotifierItem* m_trayIcon = nullptr;
};

#endif // MAINWINDOW_H
