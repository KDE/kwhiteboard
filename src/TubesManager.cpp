#include "TubesManager.h"

#include "IncomingTube.h"
#include "OutgoingTube.h"
#include "TelepathyHelpers.h"

#include <TelepathyQt4/Channel>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4/Constants>
#include <TelepathyQt4/Debug>
#include <TelepathyQt4/Types>
#include <TelepathyQt4/DBusTubeChannel>
#include <TelepathyQt4/IncomingDBusTubeChannel>
#include <TelepathyQt4/OutgoingDBusTubeChannel>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/SharedPtr>

#include <KDebug>

static inline Tp::ChannelClassList channelClassList()
{
    QMap<QString, QDBusVariant> filter;
    filter[TELEPATHY_INTERFACE_CHANNEL ".ChannelType"] =
            QDBusVariant(TELEPATHY_INTERFACE_CHANNEL_TYPE_DBUS_TUBE);
//    filter[TELEPATHY_INTERFACE_CHANNEL_TYPE_DBUS_TUBE ".ServiceName"] = QDBusVariant("org.kde.KWhiteBoard");

    QMap<QString, QDBusVariant> filter2;
    filter2[TELEPATHY_INTERFACE_CHANNEL ".ChannelType"] =
    QDBusVariant(TELEPATHY_INTERFACE_CHANNEL_TYPE_TEXT);

    return Tp::ChannelClassList() << Tp::ChannelClass(filter) << Tp::ChannelClass(filter2);
}

TubesManager::TubesManager(QObject *parent)
  : QObject(parent),
    AbstractClientHandler(channelClassList(), true)
{
    kDebug();

  //  Tp::enableDebug(true);
    Tp::enableWarnings(true);

}

TubesManager::~TubesManager()
{
    kDebug();
}

bool TubesManager::bypassApproval() const
{
    kDebug();
 
    return false;
}

void TubesManager::addRequest(const Tp::ChannelRequestPtr& request)
{
    kDebug();
    m_channelRequest = request;
}

void TubesManager::removeRequest(const Tp::ChannelRequestPtr& request, const QString& errorName, const QString& errorMessage)
{
    kDebug();
}


void TubesManager::handleChannels(const Tp::MethodInvocationContextPtr<> & context,
                                  const Tp::AccountPtr & account,
                                  const Tp::ConnectionPtr & connection,
                                  const QList<Tp::ChannelPtr> & channels,
                                  const QList<Tp::ChannelRequestPtr> & requestsSatisfied,
                                  const QDateTime & userActionTime,
                                  const QVariantMap & handlerInfo)
{
    kDebug();

    Q_FOREACH (const Tp::ChannelPtr &channel, channels) {
        kDebug() << "We have a channel...";

        QVariantMap properties = channel->immutableProperties();

        kDebug() << "ChannelType: " << properties[TELEPATHY_INTERFACE_CHANNEL ".ChannelType"];

        if (properties[TELEPATHY_INTERFACE_CHANNEL ".ChannelType"] ==
               TELEPATHY_INTERFACE_CHANNEL_TYPE_DBUS_TUBE) {

            kDebug() << "It's a DBUS Tube...";

            if (properties[TELEPATHY_INTERFACE_CHANNEL ".Requested"].toBool()) {
                kDebug() << "Outgoing.....!!!!!";
              /*  OutgoingTube *oTube = new OutgoingTube(channel);
                connect(oTube,
                        SIGNAL(tubeReady(QTcpSocket*,QString)),
                        SIGNAL(newOutgoingTube(QTcpSocket*,QString)));
                m_outgoingTubes.append(oTube);
                */
             // m_outgoingGroupDBusChannel = Tp::OutgoingDBusTubeChannel::create(channel->connection(), channel->objectPath(), channel->immutableProperties());
             m_outgoingGroupDBusChannel = Tp::OutgoingDBusTubeChannelPtr::dynamicCast(channel);
              Tp::Features oFeatures;
              oFeatures << Tp::Channel::FeatureCore << Tp::OutgoingDBusTubeChannel::FeatureDBusTube
                        << Tp::TubeChannel::FeatureTube << Tp::OutgoingDBusTubeChannel::FeatureBusNamesMonitoring;
              connect(m_outgoingGroupDBusChannel->becomeReady(oFeatures),
                      SIGNAL(finished(Tp::PendingOperation*)),
                      SLOT(onOutgoingTubeReady(Tp::PendingOperation*)));

              kDebug() << "Emitting out";
            } else {
                kDebug() << "Incoming.....!!!!!";
                /*
                IncomingTube *iTube = new IncomingTube(channel);
                connect(iTube,
                        SIGNAL(tubeReady(QTcpSocket*,QString)),
                        SIGNAL(newIncomingTube(QTcpSocket*,QString)));
                m_incomingTubes.append(iTube);
                */
                //m_incomingGroupDBusChannel = Tp::IncomingDBusTubeChannel::create(channel->connection(), channel->objectPath(), channel->immutableProperties());
                m_incomingGroupDBusChannel = Tp::IncomingDBusTubeChannelPtr::dynamicCast(channel);
                Tp::Features iFeatures;
                iFeatures << Tp::Channel::FeatureCore << Tp::IncomingDBusTubeChannel::FeatureDBusTube
                          << Tp::TubeChannel::FeatureTube << Tp::IncomingDBusTubeChannel::FeatureBusNamesMonitoring;
                connect(m_incomingGroupDBusChannel->becomeReady(iFeatures),
                        SIGNAL(finished(Tp::PendingOperation*)),
                        SLOT(onIncomingTubeReady(Tp::PendingOperation*)));
                kDebug() << "Emitting in";
            }

            m_groupDBusChannel = channel;

        }

        m_groupTextChannel = channel;
    }
    kDebug() << "Context finished";

    context->setFinished();
}

void TubesManager::onOutgoingTubeReady(Tp::PendingOperation* op)
{
    kDebug();

    connect(m_outgoingGroupDBusChannel->offerTube(QVariantMap()), SIGNAL(finished(Tp::PendingOperation*)), SLOT(onOfferTubeFinished(Tp::PendingOperation*)));
}

void TubesManager::onIncomingTubeReady(Tp::PendingOperation* op)
{
    kDebug();

    connect(m_incomingGroupDBusChannel->acceptTube(), SIGNAL(finished(Tp::PendingOperation*)), SLOT(onAcceptTubeFinished(Tp::PendingOperation*)));
}

void TubesManager::onOfferTubeFinished(Tp::PendingOperation* op)
{
    kDebug();

    Q_EMIT gotTubeDBusConnection(m_outgoingGroupDBusChannel->dbusConnection());
    Q_EMIT gotTubeChannel(m_groupDBusChannel);
}

void TubesManager::onAcceptTubeFinished(Tp::PendingOperation* op)
{
    kDebug();

    Q_EMIT gotTubeDBusConnection(m_incomingGroupDBusChannel->dbusConnection());
}



#include "TubesManager.moc"

