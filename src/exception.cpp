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

#include "exception.h"

#include <KLocalizedString>

// ---------------------------------------------------------------------------------------------- //

static auto getErrorString(Error error) -> QString
{
    switch (error)
    {
    case Error::DeviceInUse:
        return i18n("The selected virtual device is in use.");

    case Error::DeviceNotAvailable:
        return i18n("The selected virtual device is not available.");

    case Error::NoFreeDevice:
        return i18n("All virtual devices are in use.");

    case Error::FileNotFound:
        return i18n("The file doesn't exist.");

    case Error::DaemonNotRunning:
        return i18n("Unable to connect to the CDEmu daemon.");

    default:
        return i18n("An unknown error occured.");
    }
}

// ---------------------------------------------------------------------------------------------- //

Exception::Exception(Error error)
    : std::runtime_error(getErrorString(error).toLocal8Bit()) {}

// ---------------------------------------------------------------------------------------------- //
