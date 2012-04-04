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

#include <KDebug>

#define KWHITEBOARD_SERVICE_NAME QLatin1String("org.kde.KWhiteBoard")

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

    Q_FOREACH (const Tp::ChannelPtr &channel, channels) {
        kDebug() << "We have a channel...";

        QVariantMap properties = channel->immutableProperties();

        kDebug() << "ChannelType: " << properties.value(TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType"));

        if (properties.value(TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType")) ==
               TP_QT_IFACE_CHANNEL_TYPE_DBUS_TUBE) {

            kDebug() << "It's a DBUS Tube...";


            KWhiteBoard *mainWindow = new KWhiteBoard();
            mainWindow->show();

            connect(this, SIGNAL(gotTubeDBusConnection(QDBusConnection)),
                    mainWindow, SLOT(onGotTubeDBusConnection(QDBusConnection)));

            if (properties.value(TP_QT_IFACE_CHANNEL + QLatin1String(".Requested")).toBool()) {
                kDebug() << "Outgoing.....!!!!!";
              /*  OutgoingTube *oTube = new OutgoingTube(channel);
                connect(oTube,
                        SIGNAL(tubeReady(QTcpSocket*,QString)),
                        SIGNAL(newOutgoingTube(QTcpSocket*,QString)));
                m_outgoingTubes.append(oTube);
                */
             // m_outgoingGroupDBusChannel = Tp::OutgoingDBusTubeChannel::create(channel->connection(), channel->objectPath(), channel->immutableProperties());
             m_outgoingGroupDBusChannel = Tp::OutgoingDBusTubeChannelPtr::dynamicCast(channel);
              Tp::Features oFeatures;
              oFeatures << Tp::Channel::FeatureCore << Tp::OutgoingDBusTubeChannel::FeatureCore << Tp::DBusTubeChannel::FeatureCore
                        << Tp::TubeChannel::FeatureCore << Tp::Channel::FeatureConferenceInitialInviteeContacts << Tp::DBusTubeChannel::FeatureBusNameMonitoring;
              connect(m_outgoingGroupDBusChannel->becomeReady(oFeatures),
                      SIGNAL(finished(Tp::PendingOperation*)),
                      SLOT(onOutgoingTubeReady(Tp::PendingOperation*)));

              kDebug() << "Emitting out";
            } else {
                kDebug() << "Incoming.....!!!!!";
                /*
                IncomingTube *iTube = new IncomingTube(channel);
                connect(iTube,
                        SIGNAL(tubeReady(QTcpSocket*,QString)),
                        SIGNAL(newIncomingTube(QTcpSocket*,QString)));
                m_incomingTubes.append(iTube);
                */
                //m_incomingGroupDBusChannel = Tp::IncomingDBusTubeChannel::create(channel->connection(), channel->objectPath(), channel->immutableProperties());
                m_incomingGroupDBusChannel = Tp::IncomingDBusTubeChannelPtr::dynamicCast(channel);
                Tp::Features iFeatures;
                iFeatures << Tp::Channel::FeatureCore << Tp::IncomingDBusTubeChannel::FeatureCore << Tp::DBusTubeChannel::FeatureCore
                          << Tp::TubeChannel::FeatureCore << Tp::Channel::FeatureConferenceInitialInviteeContacts << Tp::DBusTubeChannel::FeatureBusNameMonitoring;
                connect(m_incomingGroupDBusChannel->becomeReady(iFeatures),
                        SIGNAL(finished(Tp::PendingOperation*)),
                        SLOT(onIncomingTubeReady(Tp::PendingOperation*)));
                kDebug() << "Emitting in";
            }
        }
    }
    kDebug() << "Context finished";

    context->setFinished();
}

void KWhiteboardHandler::onOutgoingTubeReady(Tp::PendingOperation* op)
{
    Q_UNUSED(op);

    kDebug();
    connect(m_outgoingGroupDBusChannel->offerTube(QVariantMap()),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onOfferTubeFinished(Tp::PendingOperation*)));
}

void KWhiteboardHandler::onIncomingTubeReady(Tp::PendingOperation* op)
{
    Q_UNUSED(op);

    kDebug();

    connect(m_incomingGroupDBusChannel->acceptTube(),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAcceptTubeFinished(Tp::PendingOperation*)));
}

void KWhiteboardHandler::onOfferTubeFinished(Tp::PendingOperation* op)
{
    Q_UNUSED(op);

    kDebug();

    Q_EMIT gotTubeDBusConnection(m_outgoingGroupDBusChannel->connection());
}

void KWhiteboardHandler::onAcceptTubeFinished(Tp::PendingOperation* op)
{
    Q_UNUSED(op);

    kDebug();

    Q_EMIT gotTubeDBusConnection(m_incomingGroupDBusChannel->connection());
}


#include "kwhiteboard-handler.moc"
