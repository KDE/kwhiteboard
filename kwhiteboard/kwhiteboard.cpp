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
#include "kwhiteboardwidget.h"
#include "PeerInterface.h"
#include "IntrospectableInterface.h"

KWhiteboard::KWhiteboard(const QDBusConnection& conn, QTabWidget *parent) :
    KXmlGuiWindow(parent),
    m_connection(conn)
{
    kDebug();
    setWindowIcon(KIcon(QLatin1String("applications-education")));
}

void KWhiteboard::onGotTubeDBusConnection()
{
    kDebug() << m_connection.name();

    m_whiteboardWidget = new KWhiteboardWidget(this, m_connection);
    setCentralWidget(m_whiteboardWidget);

    setupActions();
}

void KWhiteboard::setupActions()
{
    KAction* ping = new KAction(this);
    ping->setText(i18n("&Ping"));
    ping->setIcon(KIcon("document-new"));
    ping->setShortcut(Qt::CTRL + Qt::Key_P);
    actionCollection()->addAction("Ping", ping);

    connect(ping, SIGNAL(triggered(bool)),
            this, SLOT(pingBoard()));

    KStandardAction::quit(kapp, SLOT(quit()),
                            actionCollection());

    setupGUI(Default, "kwhiteboardui.rc");
}

void KWhiteboard::pingBoard()
{
    kDebug() << "ping ping!!";
    org::freedesktop::DBus::Peer *ping_iface = new org::freedesktop::DBus::Peer("", "/kwhiteboard", m_connection, this);
    kDebug() <<"My Machine ID: " << QDBusConnection::localMachineId () << "\n";
    kDebug() << "other peer's machine id" << ping_iface->GetMachineId();
    QTimer *timer = new QTimer(this);
    timer->start();
    QDBusPendingReply<QString> reply = ping_iface->Ping();
    reply.waitForFinished();
    if(reply.isValid())
    {
        kDebug() << reply.value();
        kDebug() << "No Error!";
    }
    else
    {
        kDebug() << "Error!";
    }

    kDebug() << "Total Ping time: " << timer->interval();
    timer->stop();
}

#include "kwhiteboard.moc"
