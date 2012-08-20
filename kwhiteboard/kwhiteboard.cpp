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
#include <KDE/KAction>
#include <KDE/KDebug>
#include <KDE/KLocale>
#include <KDE/KStatusBar>
#include <QtCore/QTimer>
#include <QtDBus/QDBusPendingReply>
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
    m_latencyLabel->setContentsMargins(0, 0, 5, 0);
    m_latencyLabel->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    statusBar()->addPermanentWidget(m_latencyLabel);
    setupGUI(Default, "kwhiteboardui.rc");
    startTimer(10000);
    m_timer.invalidate();
    m_latencyLabel->setText(i18n("Latency: ???"));
}

void KWhiteboard::setStatus(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<> reply = *call;
    if (reply.isError()) {
        kDebug() << reply.error();
        m_latencyLabel->setText(i18n("Latency: ???"));
    } else {
        m_latencyLabel->setText(i18n("Latency: %1 ms", m_timer.elapsed()));
    }
    call->deleteLater();
    m_timer.invalidate();
}

void KWhiteboard::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if(!m_timer.isValid())
    {
        org::kde::DBus::Peer *peerIface = new org::kde::DBus::Peer("", "/peer", m_connection, this);
        m_timer.start();
        QDBusPendingCall async = peerIface->asyncCall("Ping");
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(async, this);
        QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                             this, SLOT(setStatus(QDBusPendingCallWatcher*)));
    }
    // else previous ping is still running
}

#include "kwhiteboard.moc"
