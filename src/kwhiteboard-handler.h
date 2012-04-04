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

#ifndef KWHITEBOARD_HANDLER_H
#define KWHITEBOARD_HANDLER_H

#include <QtCore/QList>

#include <TelepathyQt/AbstractClientHandler>
#include <TelepathyQt/ChannelRequest>
#include <TelepathyQt/ChannelClassSpecList>

class IncomingTube;
class OutgoingTube;
class QTcpSocket;

class KWhiteboardHandler : public QObject, public Tp::AbstractClientHandler
{
    Q_OBJECT

public:
    KWhiteboardHandler(QObject *parent = 0);
    virtual ~KWhiteboardHandler();

    virtual bool bypassApproval() const;
    virtual void handleChannels(const Tp::MethodInvocationContextPtr<> &context,
                                const Tp::AccountPtr &account,
                                const Tp::ConnectionPtr &connection,
                                const QList<Tp::ChannelPtr> &channels,
                                const QList<Tp::ChannelRequestPtr> &requestsSatisfied,
                                const QDateTime &userActionTime,
                                const HandlerInfo &handlerInfo);

public Q_SLOTS:
    void onOutgoingTubeReady(Tp::PendingOperation *op);
    void onIncomingTubeReady(Tp::PendingOperation *op);
    void onOfferTubeFinished(Tp::PendingOperation *op);
    void onAcceptTubeFinished(Tp::PendingOperation *op);

Q_SIGNALS:
    void newIncomingTube(QTcpSocket *socket, const QString &jabberId);
    void newOutgoingTube(QTcpSocket *socket, const QString &jabberId);
    void gotTubeDBusConnection(Tp::ConnectionPtr connectionPtr);
    void gotTubeDBusConnection(const QDBusConnection &conn);
    void gotTubeChannel(Tp::ChannelPtr channel);

private:
    QList<IncomingTube*> m_incomingTubes;
    QList<OutgoingTube*> m_outgoingTubes;

    Tp::ChannelRequestPtr m_channelRequest;

    Tp::ChannelPtr m_groupTextChannel;
    Tp::IncomingDBusTubeChannelPtr m_incomingGroupDBusChannel;
    Tp::OutgoingDBusTubeChannelPtr m_outgoingGroupDBusChannel;
    Tp::ChannelPtr m_groupDBusChannel;
};


#endif // Header guard
