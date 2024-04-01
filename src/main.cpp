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

#include <KAboutData>
#include <KDBusService>
#include <KLocalizedString>

#include <QApplication>
#include <QCommandLineParser>
#include <QTextStream>

#include "cdemu.h"
#include "kdecdemuversion.h"
#include "mainwindow.h"
#include "messagebox.h"

// ---------------------------------------------------------------------------------------------- //

static void mountImage(const CDEmu& cdemu, const QString& filename)
{
    int index = cdemu.getNextFreeDevice();

    if (index < 0)
        index = cdemu.addDevice();

    cdemu.mount(QDir().absoluteFilePath(filename), index);
}

// ---------------------------------------------------------------------------------------------- //

static void unmountImage(const CDEmu& cdemu, int index)
{
    cdemu.unmount(index);
}

// ---------------------------------------------------------------------------------------------- //

static void printStatus(const CDEmu& cdemu)
{
    static constexpr const char* Tab = "\t\t";

    QTextStream out(stdout, QIODevice::WriteOnly);
    out << "Device" << Tab << "Loaded" << Tab << "Image" << Qt::endl;

    const int deviceCount = cdemu.getDeviceCount();

    for (int i = 0; i < deviceCount; ++i)
    {
        CDEmu::Status status = cdemu.getStatus(i);

        if (status.loaded)
            out << i << Tab << "Yes" << Tab << status.fileName;
        else
            out << i << Tab << "No"  << Tab << "None";

        out << Qt::endl;
    }
}

// ---------------------------------------------------------------------------------------------- //

auto main(int argc, char* argv[]) -> int
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme("media-optical"));

    KLocalizedString::setApplicationDomain("kde_cdemu");

    KAboutData aboutData(QStringLiteral("kde_cdemu"), i18n("KDE CDEmu Manager"),
                         QStringLiteral(KDE_CDEMU_VERSION), i18n("A KDE Frontend to CDEmu."),
                         KAboutLicense::GPL_V3, i18n("Copyright (c) 2009-2024 Marcel Hasler"));

    aboutData.addAuthor(i18n("Marcel Hasler"), i18n("Author and Maintainer"),
                        QStringLiteral("mahasler@gmail.com"));

    KAboutData::setApplicationData(aboutData);

    app.setApplicationName(aboutData.componentName());
    app.setApplicationDisplayName(aboutData.displayName());
    app.setApplicationVersion(aboutData.version());

    QCommandLineParser parser;

    aboutData.setupCommandLine(&parser);

    parser.setApplicationDescription(aboutData.shortDescription());

    QCommandLineOption mountOption("mount", i18n("Mount an image."), i18n("file"));
    parser.addOption(mountOption);

    QCommandLineOption unmountOption("unmount", i18n("Unmount an image."), i18n("device number"));
    parser.addOption(unmountOption);

    QCommandLineOption statusOption("status", i18n("Show information about devices."));
    parser.addOption(statusOption);

    parser.process(app);
    aboutData.processCommandLine(&parser);

    try {
        CDEmu cdemu;

        if (parser.isSet(mountOption))
            mountImage(cdemu, parser.value(mountOption));
        else if (parser.isSet(unmountOption))
            unmountImage(cdemu, parser.value(unmountOption).toInt());
        else if (parser.isSet(statusOption))
            printStatus(cdemu);
        else
        {
            // Allow only one application instance
            const KDBusService service(KDBusService::Unique);

            MainWindow window(cdemu);
            window.show();

            return QApplication::exec();
        }
    }
    catch (const Exception& e)
    {
        MessageBox::error(e.what());
        return -1;
    }

    return 0;
}

// ---------------------------------------------------------------------------------------------- //
