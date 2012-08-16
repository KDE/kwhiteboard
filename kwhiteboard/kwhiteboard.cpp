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
#include <KStatusBar>
#include <QTimer>
#include <QDBusConnection>
#include "kwhiteboardwidget.h"
#include "PeerInterface.h"

KWhiteboard::KWhiteboard(const QDBusConnection& conn, QWidget *parent) :
    KXmlGuiWindow(parent),
    m_connection(conn)
{
    kDebug() << m_connection.name();
    setWindowIcon(KIcon(QLatin1String("applications-education")));

    m_whiteboardWidget = new KWhiteboardWidget(this, m_connection);
    setCentralWidget(m_whiteboardWidget);

    m_latencyLabel = new QLabel(this);
    statusBar()->addPermanentWidget(m_latencyLabel);
    setupGUI(Default, "kwhiteboardui.rc");
    startTimer(10000);
}

int KWhiteboard::latencyValue()
{
    org::kde::DBus::Peer *peerIface = new org::kde::DBus::Peer("", "/peer", m_connection, this);
    QTime *timer = new QTime();
    timer->start();
    QDBusPendingReply<> reply = peerIface->asyncCall("Ping");
    reply.waitForFinished();
    if(!reply.isValid())
    {
        kDebug() << "Error in calculating the latency!";
    }
    return timer->elapsed();
}

void KWhiteboard::timerEvent(QTimerEvent *event)
{
    QString time = "Latency: ";
    time.append(QString::number(latencyValue()));
    m_latencyLabel->setText(time);
}

#include "kwhiteboard.moc"
