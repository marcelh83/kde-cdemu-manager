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

#ifndef DEVICELISTITEM_H
#define DEVICELISTITEM_H

#include <QLabel>
#include <QPushButton>
#include <QTreeWidgetItem>

class DeviceListItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT

public:
    DeviceListItem(int index);

    void setFileName(const QString& name);
    auto fileName() const -> QString;

    auto widget() const -> QWidget*;

signals:
    void mountClicked(int index);
    void unmountClicked(int index);

private slots:
    void onButtonClicked();

private:
    int m_index;

    QWidget* m_widget;
    QLabel* m_label;
    QPushButton* m_button;
};

#endif // DEVICELISTITEM_H
