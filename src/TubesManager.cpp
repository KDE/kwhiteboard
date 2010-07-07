#include "TubesManager.h"

#include "IncomingTube.h"
#include "OutgoingTube.h"
#include "TelepathyHelpers.h"

#include <TelepathyQt4/Channel>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4/Constants>
#include <TelepathyQt4/Debug>
#include <TelepathyQt4/IncomingDBusTubeChannel>
#include <TelepathyQt4/OutgoingDBusTubeChannel>

#include <KDebug>

static inline Tp::ChannelClassList channelClassList()
{
    QMap<QString, QDBusVariant> filter;
    filter[TELEPATHY_INTERFACE_CHANNEL ".ChannelType"] =
            QDBusVariant(TELEPATHY_INTERFACE_CHANNEL_TYPE_DBUS_TUBE);
    filter[TELEPATHY_INTERFACE_CHANNEL_TYPE_DBUS_TUBE ".ServiceName"] = QDBusVariant("org.kde.KWhiteBoard");

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
              Tp::OutgoingDBusTubeChannelPtr oTube = Tp::OutgoingDBusTubeChannel::create(channel->connection(), channel->objectPath(), channel->immutableProperties());
              connect(oTube->offerTube(QVariantMap()), SIGNAL(finished(Tp::PendingOperation*)), SLOT(onOfferTubeFinished(Tp::PendingOperation*)));
              m_outgoingGroupDBusChannel = oTube;

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
                Tp::IncomingDBusTubeChannelPtr iTube = Tp::IncomingDBusTubeChannel::create(channel->connection(), channel->objectPath(), channel->immutableProperties());
                connect(iTube->acceptTube(), SIGNAL(finished(Tp::PendingOperation*)), SLOT(onAcceptTubeFinished(Tp::PendingOperation*)));
                m_incomingGroupDBusChannel = iTube;
                kDebug() << "Emitting in";
            }

            m_groupDBusChannel = channel;

        }

        m_groupTextChannel = channel;
    }
    kDebug() << "Context finished";

    context->setFinished();
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

