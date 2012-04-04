/*
 *  Copyright (C) 2010 David Faure   <faure@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation;
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "kwhiteboard.h"
#include <KAction>
#include <KActionCollection>
#include <KLocale>
#include <KDebug>
#include "kwhiteboardwidget.h"

KWhiteboard::KWhiteboard()
{
    kDebug();
    setWindowIcon(KIcon(QLatin1String("applications-education")));
}

void KWhiteboard::onGotTubeDBusConnection(const QDBusConnection& conn)
{
    kDebug() << conn.name();

    m_whiteboardWidget = new KWhiteboardWidget(this, conn);
    setCentralWidget(m_whiteboardWidget);

    setupGUI();
}


#include "kwhiteboard.moc"
