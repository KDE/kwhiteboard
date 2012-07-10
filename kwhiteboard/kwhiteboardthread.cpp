/*
 *  Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#include "kwhiteboardthread.h"

static const char* s_objectPath = "/kwhiteboard";
static const char* s_dbusInterface = "org.kde.KWhiteboard";


KWhiteboardThread::KWhiteboardThread(const QDBusConnection &conn, QObject* parent) :
    QThread(parent),
    m_connection(conn)
{

    // Connect the remote signal to the drawLine method
    // No service specified so that we connect to the signal from *all* of this object on the bus.
    m_connection.connect(QString(), s_objectPath, s_dbusInterface, "sigDrawLine", this, SIGNAL(sigDrawLine(int, int, int, int)));

}

KWhiteboardThread::~KWhiteboardThread()
{

}
