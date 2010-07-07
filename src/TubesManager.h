
#ifndef TUBES_MANAGER_H
#define TUBES_MANAGER_H

#include <QtCore/QList>

#include <TelepathyQt4/AbstractClientHandler>
#include <TelepathyQt4/ChannelRequest>

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
                                const QVariantMap &handlerInfo);
    virtual void addRequest(const Tp::ChannelRequestPtr &request);
    virtual void removeRequest(const Tp::ChannelRequestPtr &request, const QString &errorName, const QString &errorMessage);

Q_SIGNALS:
    void newIncomingTube(QTcpSocket *socket, const QString &jabberId);
    void newOutgoingTube(QTcpSocket *socket, const QString &jabberId);
    void gotTubeDBusConnection(const QDBusConnection &conn);
    void gotTubeChannel(Tp::ChannelPtr channel);

private:
    QList<IncomingTube*> m_incomingTubes;
    QList<OutgoingTube*> m_outgoingTubes;

    Tp::ChannelRequestPtr m_channelRequest;

    Tp::ChannelPtr m_groupTextChannel;
    Tp::ChannelPtr m_groupDBusChannel;
};


#endif // Header guard

