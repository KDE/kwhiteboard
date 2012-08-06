/*
 *  Copyright (C) 2012 Puneet Goyal   <puneetgoyal08@gmail.com>
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

#include <KDebug>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QDBusConnection>
#include <QDBusInterface>

#include "PeerInterface.h"
#include "PeerAdaptor.h"
#include "PeerManager.h"

static const char* s_objectPath = "/Peer";

PeerManager::PeerManager(QObject* parent,const QDBusConnection &conn)
    : QObject(parent),
      m_connection(conn)
{
    kDebug() << parent;
	new PeerAdaptor(this);
    if (!m_connection.registerObject(QString::fromLatin1(s_objectPath), this, QDBusConnection::ExportAllSignals | QDBusConnection::ExportAllSlots| QDBusConnection::ExportAdaptors)) {
        kWarning() << parent << "Could not register object on DBus connection" << m_connection.name();
        qApp->exit(1);
    } else {
        kDebug() << parent << "Object registered on DBus connection" << m_connection.name();
    }
}

void PeerManager::Ping()
{
    kDebug()<< "Ping function is called";
}

QByteArray PeerManager::GetMachineId()
{
    return QDBusConnection::localMachineId ();
}

#include "PeerManager.moc"
