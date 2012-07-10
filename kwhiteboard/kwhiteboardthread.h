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


#ifndef KWHITEBOARDTHREAD_H
#define KWHITEBOARDTHREAD_H

#include <QtCore/QThread>
#include <QtDBus/QDBusConnection>

class KWhiteboardWidget;
class KWhiteboardThread : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(KWhiteboardThread)

public:
    explicit KWhiteboardThread(const QDBusConnection &conn, QObject* parent = 0);
    virtual ~KWhiteboardThread();

Q_SIGNALS:
    void sigDrawLine(int x1, int y1, int x2, int y2);

private:
    QDBusConnection m_connection;
};


#endif // KWHITEBOARDTHREAD_H