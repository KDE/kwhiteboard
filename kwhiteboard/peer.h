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

#ifndef PEER_H
#define PEER_H

#include <QtCore/QObject>

class QDBusConnection;

class Peer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Peer)

public:
    Peer(QDBusConnection &connection, QObject *parent = 0);

public Q_SLOTS:
    QString GetMachineId();
    void Ping();
};

#endif // PEER_H
