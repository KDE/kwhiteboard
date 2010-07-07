#ifndef TELEPATHY_CHANNEL_MANAGER_H
#define TELEPATHY_CHANNEL_MANAGER_H

#include <QObject>
#include <QtCore/QString>
#include <QtNetwork/QTcpSocket>

#include <TelepathyQt4/ClientRegistrar>
#include <TelepathyQt4/Channel>

class TubesManager;

class TelepathyChannelManager : public QObject
{
    Q_OBJECT
    
public:
    TelepathyChannelManager(QObject *parent = 0);
    ~TelepathyChannelManager();
    
    void initialize();
    
    /**
     * Call this method with when you want to start a channel to
     * someone, with their Jabber ID as the jabberID parameter.
     *
     * This is an asynchronous action. This method does not return
     * anything, but once the Channel is ready for you to use,
     * the newOutgoingChannel() signal will be emitted.
     */
    void startChannel(const QString &jabberId);
    
Q_SIGNALS:
    /**
     * Once a channel you have requested is ready to use, this
     * signal will be emitted, with the QTcpSocket which you can
     * write to, and the jabberID of the person you requested the
     * channel to.
     */
    void newOutgoingChannel(QTcpSocket *socket, const QString &jabberId);
    
    /**
     * When someone else requests a channel to you, this signal is emitted
     * with a QTcpSocket which you can read from/write to, and their
     * jabber ID.
     */
    void newIncomingChannel(QTcpSocket *socket, const QString &jabberId);

    void gotTubeDBusConnection(const QDBusConnection &conn);

    void gotTubeChannel(Tp::ChannelPtr channel);
    
private:
    Tp::ClientRegistrarPtr m_registrar;
    Tp::SharedPtr<TubesManager> m_tubesManager;

};


#endif // Header guard

