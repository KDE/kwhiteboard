#include "telepathyblackboard.h"

// Qt headers
#include <QtGui/QWidget>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include <KCmdLineArgs>


#include <TelepathyQt/ChannelClassSpec>
#include <TelepathyQt/PendingStreamTubeConnection>
#include <TelepathyQt/IncomingStreamTubeChannel>
#include <TelepathyQt/OutgoingStreamTubeChannel>


#include "blackboardwidget.h"


TelepathyBlackboard::TelepathyBlackboard()
    : QObject(),
      Tp::AbstractClientHandler(Tp::ChannelClassSpecList() << Tp::ChannelClassSpec::incomingStreamTube("kwhiteboard")
                                                           << Tp::ChannelClassSpec::outgoingStreamTube("kwhiteboard")),
          m_server(0)
{
    qDebug() << "waiting for connections";
}

TelepathyBlackboard::~TelepathyBlackboard()
{
}

void TelepathyBlackboard::onNewConnectionAvaiable()
{
    QTcpSocket* socket = m_server->nextPendingConnection();
    if (socket) {
        m_whiteboardWidget = new BlackboardWidget();
        m_whiteboardWidget->setSocket(socket);
        m_whiteboardWidget->show();
    }
}



bool TelepathyBlackboard::bypassApproval() const
{
    return true;
}

void TelepathyBlackboard::handleChannels(const Tp::MethodInvocationContextPtr<> &context, const Tp::AccountPtr &account, const Tp::ConnectionPtr &connection, const QList<Tp::ChannelPtr> &channels, const QList<Tp::ChannelRequestPtr> &requestsSatisfied, const QDateTime &userActionTime, const Tp::AbstractClientHandler::HandlerInfo &handlerInfo)
{
    Q_UNUSED(account);
    Q_UNUSED(connection);
    Q_UNUSED(requestsSatisfied);
    Q_UNUSED(userActionTime);
    Q_UNUSED(handlerInfo);

    foreach(const Tp::ChannelPtr &channel, channels) {
        qDebug() << "Handling new stream tube";

        if (Tp::IncomingStreamTubeChannelPtr incomingStreamTube = Tp::IncomingStreamTubeChannelPtr::dynamicCast(channel)) {
            qDebug() << "incoming";
            Tp::PendingStreamTubeConnection* pendingConnection = incomingStreamTube->acceptTubeAsTcpSocket();
            connect(pendingConnection, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onPendingIncomingConnectionReady(Tp::PendingOperation*)));

        } else if (Tp::OutgoingStreamTubeChannelPtr outgoingFileTransferChannel = Tp::OutgoingStreamTubeChannelPtr::dynamicCast(channel)) {
            qDebug() << "outgoing";
            m_server = new QTcpServer(this);
            m_server->listen(QHostAddress::Any, 0);
            outgoingFileTransferChannel->offerTcpSocket(m_server, QVariantMap());
            connect(m_server, SIGNAL(newConnection()), SLOT(onNewConnectionAvaiable()));
        }
    }
    context->setFinished();
}

void TelepathyBlackboard::onPendingIncomingConnectionReady(Tp::PendingOperation *op)
{
    Tp::PendingStreamTubeConnection *pendingConnection = qobject_cast<Tp::PendingStreamTubeConnection*>(op);
    Q_ASSERT(pendingConnection);


    qDebug() << pendingConnection->ipAddress();

    QTcpSocket* socket = new QTcpSocket(this);
    socket->connectToHost(pendingConnection->ipAddress().first, pendingConnection->ipAddress().second);

    m_whiteboardWidget = new BlackboardWidget();
    m_whiteboardWidget->setSocket(socket);
    m_whiteboardWidget->show();
}


#include "moc_telepathyblackboard.cpp"

