#ifndef INCOMING_TUBE_H
#define INCOMING_TUBE_H

#include "TelepathyHelpers.h"

#include <QtCore/QObject>

#include <TelepathyQt/Channel>

namespace Tp {
    class PendingOperation;
}

class QTcpSocket;

class IncomingTube : public QObject
{
    Q_OBJECT
    
public:
    IncomingTube(const Tp::ChannelPtr &channel, QObject *parent = 0);
    ~IncomingTube();

Q_SIGNALS:
    void tubeReady(QTcpSocket *socket, const QString &jabberId);

private Q_SLOTS:
    void onChannelReady(Tp::PendingOperation *op);
    void onTubeStateChanged(uint state);
    void onTcpSocketConnected();
    
private:
    Tp::ChannelPtr m_channel;
    Tp::Client::ChannelTypeStreamTubeInterface *m_streamTubeInterface;
    Tp::Client::ChannelInterfaceTubeInterface *m_tubeInterface;
    StreamTubeAddress m_streamTubeAddress;
    QTcpSocket *m_tcpSocket;
    QString m_jabberId;
};


#endif // Header guard

