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
#include <KApplication>
#include <KStatusBar>
#include <QTimer>
#include <QDBusConnection>
#include "kwhiteboardwidget.h"
#include "PeerInterface.h"

KWhiteboard::KWhiteboard(QTabWidget *parent) :
    KXmlGuiWindow(parent),
    m_connection(QDBusConnection(""))
{
    kDebug();
    setWindowIcon(KIcon(QLatin1String("applications-education")));
}

void KWhiteboard::onGotTubeDBusConnection(const QDBusConnection& conn)
{
    m_connection = QDBusConnection(conn.name());
    kDebug() << m_connection.name();

    m_whiteboardWidget = new KWhiteboardWidget(this, m_connection);
    setCentralWidget(m_whiteboardWidget);

    latencyLabel = new QLabel(this);
    statusBar()->addPermanentWidget(latencyLabel);
    setupActions();
    checkLatency();
}

void KWhiteboard::setupActions()
{
    KAction* ping = new KAction(this);
    ping->setText(i18n("&Ping"));
    ping->setIcon(KIcon("document-new"));
    ping->setShortcut(Qt::CTRL + Qt::Key_P);
    actionCollection()->addAction("Ping", ping);

    connect(ping, SIGNAL(triggered(bool)),
            this, SLOT(checkLatency()));

    KStandardAction::quit(kapp, SLOT(quit()),
                            actionCollection());

    setupGUI(Default, "kwhiteboardui.rc");
}

void KWhiteboard::checkLatency()
{
    org::kde::DBus::Peer *peerIface = new org::kde::DBus::Peer("", "/peer", m_connection, this);
    QTime *timer = new QTime();
    timer->start();
    QDBusPendingReply<> reply = peerIface->Ping();
    reply.waitForFinished();
    if(!reply.isValid())
    {
        kDebug() << "Error in calculating the latency!";
    }
    QString time = "Latency: ";
    time.append(QString::number(timer->elapsed()));
    latencyLabel->setText(time);
    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(checkLatency()));
    timer2->start(10000);
}

#include "kwhiteboard.moc"
