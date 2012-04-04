#include "TelepathyChannelManager.h"

#include "TubesManager.h"

TelepathyChannelManager::TelepathyChannelManager(QObject *parent)
 : QObject(parent),
   m_registrar(0),
   m_tubesManager(new TubesManager(this))
{
    connect(m_tubesManager.data(),
            SIGNAL(newIncomingTube(QTcpSocket*,QString)),
            SIGNAL(newIncomingChannel(QTcpSocket*,QString)));
    connect(m_tubesManager.data(),
            SIGNAL(newOutgoingTube(QTcpSocket*,QString)),
            SIGNAL(newOutgoingChannel(QTcpSocket*,QString)));
    connect(m_tubesManager.data(),
            SIGNAL(gotTubeDBusConnection(QDBusConnection)),
            SIGNAL(gotTubeDBusConnection(QDBusConnection)));
    connect(m_tubesManager.data(),
            SIGNAL(gotTubeChannel(Tp::ChannelPtr)),
            SIGNAL(gotTubeChannel(Tp::ChannelPtr)));
}

TelepathyChannelManager::~TelepathyChannelManager()
{
    // TODO: Implement me!
}

void TelepathyChannelManager::initialize()
{
    // Register types for TelepathyQt.
    Tp::registerTypes();

    // Set up the Telepathy Client Registrar.
    m_registrar = Tp::ClientRegistrar::create();
    m_registrar->registerClient(Tp::AbstractClientPtr::dynamicCast(m_tubesManager), "KTp.KWhiteboard");
}

void TelepathyChannelManager::startChannel(const QString &jabberId)
{
    // TODO: Implement me!
}


#include "TelepathyChannelManager.moc"

