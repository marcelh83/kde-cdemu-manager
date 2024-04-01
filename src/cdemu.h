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

#ifndef CDEMU_H
#define CDEMU_H

#include "exception.h"

#include <QtDBus>

class CDEmu : public QObject
{
    Q_OBJECT

public:
    struct Status
    {
        bool loaded;
        QString fileName;
    };

public:
    CDEmu();

    auto isDaemonRunning() const -> bool;

    auto getDeviceCount() const -> int;
    auto getNextFreeDevice() const -> int;

    auto getStatus(int index) const -> Status;

    auto isLoaded(int index) const -> bool;
    auto getFileName(int index) const -> QString;

    void mount(const QString& filename, int index) const;
    void unmount(int index) const;

    auto addDevice() const -> int;
    void removeDevice() const;

signals:
    void daemonChanged(bool running);

    void deviceAdded();
    void deviceRemoved();
    void deviceChanged(int index);

private slots:
    void onServiceRegistered(const QString& service);
    void onServiceUnregistered(const QString& service);

private:
    void connectMethod(const QString& name, const char* slot);

    auto callMethod(const QDBusMessage& method) const -> QDBusMessage;
    auto callMethod(const QString& method) const -> QDBusMessage;

    static auto createMethodCall(const QString& method) -> QDBusMessage;

private:
    QDBusServiceWatcher m_watcher;
};

#endif // CDEMU_H
