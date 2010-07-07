#include "IncomingTube.h"

#include <KDebug>

#include <QtNetwork/QTcpSocket>

#include <TelepathyQt4/Feature>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingReady>

IncomingTube::IncomingTube(const Tp::ChannelPtr &channel, QObject *parent)
 : QObject(parent),
   m_channel(channel),
   m_streamTubeInterface(0),
   m_tubeInterface(0),
   m_tcpSocket(0)
{
    kDebug();

    // Get the channel ready with all our desired features.
    Tp::Features features;
    features << Tp::Channel::FeatureCore;
    
    connect(m_channel->becomeReady(features),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onChannelReady(Tp::PendingOperation*)));
}

IncomingTube::~IncomingTube()
{
    kDebug();
    
    // TODO: Implement me!
}

void IncomingTube::onChannelReady(Tp::PendingOperation *op)
{
    kDebug();

    if (op->isError()) {
        kWarning() << "Failed to get channel ready.";
        return;
    }

    // Tube interfaces.
    m_tubeInterface = m_channel->tubeInterface();
    m_streamTubeInterface = m_channel->streamTubeInterface();
    
    if (m_streamTubeInterface && m_tubeInterface) {
        kDebug() << "We have tube control interfaces. Continue.";

        connect(m_tubeInterface,
                SIGNAL(TubeChannelStateChanged(uint)),
                SLOT(onTubeStateChanged(uint)));

        // Accept the stream tube.
        QDBusVariant ret = m_streamTubeInterface->Accept(
                Tp::SocketAddressTypeIPv4,
                Tp::SocketAccessControlLocalhost,
                QDBusVariant(""));

        // Get the stream tube address so that we know where to connect to it.
        StreamTubeAddress m_streamTubeAddress = qdbus_cast<StreamTubeAddress>(ret.variant());
        kDebug() << "Stream Tube address:"
                 << m_streamTubeAddress.address
                 << ":"
                 << m_streamTubeAddress.port;
                 
        
        // Connect a QTcpSocket to the tube, then wait for the tube state to become OPEN.
        m_tcpSocket = new QTcpSocket;
        connect(m_tcpSocket,
                SIGNAL(connected()),
                SLOT(onTcpSocketConnected()));

        m_tcpSocket->connectToHost(m_streamTubeAddress.address, m_streamTubeAddress.port);

       // TODO: Get the jabber ID of the person we are connected to.
    }
}

void IncomingTube::onTubeStateChanged(uint state)
{
    kDebug() << state;

    if (state == Tp::TubeStateOpen) {
        emit tubeReady(m_tcpSocket, m_jabberId);
    }
}

void IncomingTube::onTcpSocketConnected()
{
    kDebug();

    // TODO: Implement me!
}


#include "IncomingTube.moc"

