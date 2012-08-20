/*
 * Copyright (C) 2009-2010 Collabora Ltd. <info@collabora.co.uk>
 *   @author George Goldberg <george.goldberg@collabora.co.uk>
 * Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "kwhiteboard-handler.h"
#include "kwhiteboard.h"
#include "peer.h"

#include <TelepathyQt/Channel>
#include <TelepathyQt/Connection>
#include <TelepathyQt/Constants>
#include <TelepathyQt/Types>
#include <TelepathyQt/DBusTubeChannel>
#include <TelepathyQt/IncomingDBusTubeChannel>
#include <TelepathyQt/OutgoingDBusTubeChannel>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/SharedPtr>
#include <TelepathyQt/PendingDBusTubeConnection>
#include <TelepathyQt/ChannelClassSpec>

#include <KDE/KLocale>
#include <KDE/KDebug>

#include <KTp/telepathy-handler-application.h>


#define KWHITEBOARD_SERVICE_NAME QLatin1String("org.kde.KWhiteboard")

static inline Tp::ChannelClassSpecList channelClassSpecList()
{
    return (Tp::ChannelClassSpecList() << Tp::ChannelClassSpec::incomingDBusTube(KWHITEBOARD_SERVICE_NAME)
                                       << Tp::ChannelClassSpec::outgoingDBusTube(KWHITEBOARD_SERVICE_NAME)
                                       << Tp::ChannelClassSpec::incomingRoomDBusTube(KWHITEBOARD_SERVICE_NAME)
                                       << Tp::ChannelClassSpec::outgoingRoomDBusTube(KWHITEBOARD_SERVICE_NAME));
}

KWhiteboardHandler::KWhiteboardHandler(QObject *parent)
  : QObject(parent),
    AbstractClientHandler(channelClassSpecList(), Capabilities(), true)
{
    kDebug();
}

KWhiteboardHandler::~KWhiteboardHandler()
{
    kDebug();
}

bool KWhiteboardHandler::bypassApproval() const
{
    kDebug();

    return false;
}

void KWhiteboardHandler::handleChannels(const Tp::MethodInvocationContextPtr<> & context,
                                  const Tp::AccountPtr & account,
                                  const Tp::ConnectionPtr & connection,
                                  const QList<Tp::ChannelPtr> & channels,
                                  const QList<Tp::ChannelRequestPtr> & requestsSatisfied,
                                  const QDateTime & userActionTime,
                                  const HandlerInfo & handlerInfo)
{
    Q_UNUSED(account);
    Q_UNUSED(connection);
    Q_UNUSED(requestsSatisfied);
    Q_UNUSED(userActionTime);
    Q_UNUSED(handlerInfo);

    kDebug();

    Q_FOREACH(const Tp::ChannelPtr &channel, channels) {
        QVariantMap properties = channel->immutableProperties();

        //Q_ASSERT (properties.value(TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType")) == TP_QT_IFACE_CHANNEL_TYPE_DBUS_TUBE);
        if (properties.value(TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType")) != TP_QT_IFACE_CHANNEL_TYPE_DBUS_TUBE) {
            kWarning() << "Cannot handle channels of type" << properties.value(TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType"));
            continue;
        }

        kDebug() << "It's a DBUS Tube:" << properties.value(TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType"));

        if (KTp::TelepathyHandlerApplication::newJob() >= 0) {
            context->setFinished();
            connect(channel.data(), SIGNAL(invalidated(Tp::DBusProxy*,QString,QString)), SLOT(onChannelInvalidated()));
        } else {
            context->setFinishedWithError(QLatin1String("org.freedesktop.Telepathy.KDE.KWhiteboard.Exiting"),
                                        i18n("KWhiteboard is exiting. Cannot start job"));
            return;
        }



        if (properties.value(TP_QT_IFACE_CHANNEL + QLatin1String(".Requested")).toBool()) {
            kDebug() << "Outgoing.....!!!!!";
            m_outgoingGroupDBusChannel = Tp::OutgoingDBusTubeChannelPtr::dynamicCast(channel);
            Tp::PendingDBusTubeConnection *pendingConnection = m_outgoingGroupDBusChannel->offerTube(QVariantMap());
            connect(pendingConnection,
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(onOfferTubeFinished(Tp::PendingOperation*)));
        } else {
            kDebug() << "Incoming.....!!!!!";
            m_incomingGroupDBusChannel = Tp::IncomingDBusTubeChannelPtr::dynamicCast(channel);
            Tp::PendingDBusTubeConnection *pendingConnection = m_incomingGroupDBusChannel->acceptTube();
            connect(pendingConnection,
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(onAcceptTubeFinished(Tp::PendingOperation*)));
        }
    }

    kDebug() << "Context finished";
    context->setFinished();
}

void KWhiteboardHandler::onOfferTubeFinished(Tp::PendingOperation *op)
{
    kDebug();

    if (op->isError()) {
        kWarning() << op->errorName() << op->errorMessage();
    }

    Tp::PendingDBusTubeConnection *dbusop = qobject_cast<Tp::PendingDBusTubeConnection*>(op);
    if (!dbusop) {
        kWarning() << "This is not a PendingDBusTubeConnection";
    }

    QDBusConnection conn = QDBusConnection::connectToPeer(dbusop->address(), dbusop->address().right(8));
    kDebug() << "conn ->" << conn.name();
    new Peer(conn, this);
    KWhiteboard *mainWindow = new KWhiteboard(conn);
    mainWindow->show();
    kDebug() << "Main window created";
}

void KWhiteboardHandler::onAcceptTubeFinished(Tp::PendingOperation *op)
{
    kDebug();

    if (op->isError()) {
        kWarning() << op->errorName() << op->errorMessage();
    }

    Tp::PendingDBusTubeConnection *dbusop = qobject_cast<Tp::PendingDBusTubeConnection*>(op);
    if (!dbusop) {
        kWarning() << "This is not a PendingDBusTubeConnection";
    }

    QDBusConnection conn = QDBusConnection::connectToPeer(dbusop->address(), dbusop->address().right(8));
    kDebug() << "conn ->" << conn.name();
    new Peer(conn, this);
    KWhiteboard *mainWindow = new KWhiteboard(conn);
    mainWindow->show();
    kDebug() << "Main window created";
}

void KWhiteboardHandler::onChannelInvalidated()
{
    KTp::TelepathyHandlerApplication::jobFinished();
}


#include "kwhiteboard-handler.moc"
