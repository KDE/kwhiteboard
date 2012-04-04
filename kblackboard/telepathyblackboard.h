#ifndef TELEPATHY_WHITEBOARD_H
#define TELEPATHY_WHITEBOARD_H

#include <KDE/KApplication>

#include <TelepathyQt/AbstractClientHandler>
#include <TelepathyQt/MethodInvocationContext>


// Forward declaration
// Qt
class QTcpServer;
class QTcpSocket;

class BlackboardWidget;

namespace Tp{
    class PendingOperation;
};

class TelepathyBlackboard: public QObject, public Tp::AbstractClientHandler
{
    Q_OBJECT
public:
    TelepathyBlackboard();
    ~TelepathyBlackboard();

    virtual void handleChannels(const Tp::MethodInvocationContextPtr<> &context,
                                const Tp::AccountPtr &account,
                                const Tp::ConnectionPtr &connection,
                                const QList<Tp::ChannelPtr> &channels,
                                const QList<Tp::ChannelRequestPtr> &requestsSatisfied,
                                const QDateTime &userActionTime,
                                const Tp::AbstractClientHandler::HandlerInfo &handlerInfo);


    virtual bool bypassApproval() const;


private slots:
    void onNewConnectionAvaiable();
    void onPendingIncomingConnectionReady(Tp::PendingOperation*);

private:
    QTcpServer *m_server;
    BlackboardWidget *m_whiteboardWidget;
};

#endif
