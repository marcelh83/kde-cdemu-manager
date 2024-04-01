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

#include <KLocalizedString>

#include <QHBoxLayout>

DeviceListItem::DeviceListItem(int index)
    : m_index(index),
      m_widget(new QWidget),
      m_label(new QLabel),
      m_button(new QPushButton)
{
    setFlags(Qt::NoItemFlags);

    m_button->setFixedWidth(30);
    m_button->setFlat(true);

    connect(m_button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));

    auto layout = new QHBoxLayout(m_widget);
    layout->addWidget(m_label);
    layout->addWidget(m_button);
    layout->setContentsMargins(0, 0, 0, 0);
    m_widget->setLayout(layout);
}

// ---------------------------------------------------------------------------------------------- //

void DeviceListItem::setFileName(const QString& name)
{
    m_label->setText(name);

    if (name.isEmpty())
    {
        m_button->setIcon(QIcon::fromTheme("document-open"));
        m_button->setToolTip(i18n("Select image file"));
    }
    else
    {
        m_button->setIcon(QIcon::fromTheme("media-eject"));
        m_button->setToolTip(i18n("Unmount current image"));

    }
}

// ---------------------------------------------------------------------------------------------- //

auto DeviceListItem::fileName() const -> QString
{
    return m_label->text();
}

// ---------------------------------------------------------------------------------------------- //

auto DeviceListItem::widget() const -> QWidget*
{
    return m_widget;
}

// ---------------------------------------------------------------------------------------------- //

void DeviceListItem::onButtonClicked()
{
    if (m_label->text().isEmpty())
        emit mountClicked(m_index);
    else
        emit unmountClicked(m_index);
}

// ---------------------------------------------------------------------------------------------- //
