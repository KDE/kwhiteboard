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

#include "peer.h"
#include "PeerAdaptor.h"

#include <QtCore/QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <KDE/KDebug>

static const QLatin1String s_peerObjectPath("/peer");

Peer::Peer(QDBusConnection &connection, QObject *parent) : QObject(parent)
{
    new PeerAdaptor(this);

    if (!connection.registerObject(s_peerObjectPath,
                                   this,
                                   QDBusConnection::ExportAdaptors)) {
        kWarning() << parent << "Could not register peer object on DBus connection" << connection.name();
        qApp->exit(1);
    } else {
        kDebug() << parent << "Peer object registered on DBus connection" << connection.name();
    }
}

QString Peer::GetMachineId()
{
    return QDBusConnection::localMachineId();
}

void Peer::Ping()
{
    kDebug() << "Ping";
}
