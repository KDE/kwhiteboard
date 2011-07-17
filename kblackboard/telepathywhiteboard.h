#ifndef TELEPATHY_WHITEBOARD_H
#define TELEPATHY_WHITEBOARD_H

#include <KDE/KApplication>

#include <TelepathyQt4/AbstractClientHandler>
#include <TelepathyQt4/MethodInvocationContext>


// Forward declaration
// Qt
class QTcpServer;
class QTcpSocket;

class WhiteboardWidget;

namespace Tp{
    class PendingOperation;
};

class TelepathyWhiteboard: public QObject, public Tp::AbstractClientHandler
{
    Q_OBJECT
public:
    TelepathyWhiteboard();
    ~TelepathyWhiteboard();

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
    WhiteboardWidget *m_whiteboardWidget;
};

#endif
