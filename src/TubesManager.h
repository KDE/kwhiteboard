
#ifndef TUBES_MANAGER_H
#define TUBES_MANAGER_H

#include <QtCore/QList>

#include <TelepathyQt/AbstractClientHandler>
#include <TelepathyQt/ChannelRequest>
#include <TelepathyQt/ChannelClassSpecList>

class IncomingTube;
class OutgoingTube;
class QTcpSocket;

class TubesManager : public QObject, public Tp::AbstractClientHandler
{
    Q_OBJECT

public:
    TubesManager(QObject *parent = 0);
    virtual ~TubesManager();

    virtual bool bypassApproval() const;
    virtual void handleChannels(const Tp::MethodInvocationContextPtr<> &context,
                                const Tp::AccountPtr &account,
                                const Tp::ConnectionPtr &connection,
                                const QList<Tp::ChannelPtr> &channels,
                                const QList<Tp::ChannelRequestPtr> &requestsSatisfied,
                                const QDateTime &userActionTime,
                                const HandlerInfo &handlerInfo);
    virtual void addRequest(const Tp::ChannelRequestPtr &request);
    virtual void removeRequest(const Tp::ChannelRequestPtr &request, const QString &errorName, const QString &errorMessage);

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
    Tp::AbstractClientHandler::Capabilities m_capabilities;
    Tp::ChannelClassSpecList m_channelClassSpecList;
};


#endif // Header guard
