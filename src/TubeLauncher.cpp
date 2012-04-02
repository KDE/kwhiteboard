#include "TubeLauncher.h"

#include <KDebug>

#include <TelepathyQt/AccountManager>
#include <TelepathyQt/AccountSet>
#include <TelepathyQt/Channel>
#include <TelepathyQt/Feature>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/PendingContacts>
#include <TelepathyQt/DBusTubeChannel>
#include <TelepathyQt/OutgoingDBusTubeChannel>

TubeLauncher::TubeLauncher(QObject *parent)
  :QObject(parent)
{
    kDebug();

    // Start getting the TP stuff ready.
    m_accountManager = Tp::AccountManager::create();

    connect(m_accountManager->becomeReady(Tp::AccountManager::FeatureCore),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountManagerReady(Tp::PendingOperation*)));
}

TubeLauncher::~TubeLauncher()
{
    kDebug();
}

void TubeLauncher::onAccountManagerReady(Tp::PendingOperation* op)
{
    kDebug();

    // Get the first of the online accounts HACK!!!
    kDebug() << "No. of accounts online:" << m_accountManager->onlineAccountsSet()->accounts().size();
    m_account = m_accountManager->onlineAccountsSet()->accounts().first();

    // Get it ready.
    Tp::Features features;
    features << Tp::Account::FeatureCore
             << Tp::Account::FeatureAvatar
             << Tp::Account::FeatureProtocolInfo;

    connect(m_account->becomeReady(features), SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountReady(Tp::PendingOperation*)));

    kDebug() << "Account object:" << m_account.data() << m_account->objectPath();
}

void TubeLauncher::onAccountReady(Tp::PendingOperation* op)
{
    kDebug();

    // Get the connection to the account
    m_connection = m_account->connection();

    // Get it ready
    Tp::Features features;
    features << Tp::Connection::FeatureCore
             << Tp::Connection::FeatureRoster
             << Tp::Connection::FeatureRosterGroups
             << Tp::Connection::FeatureSelfContact
             << Tp::Connection::FeatureSimplePresence;

    connect(m_connection->becomeReady(features),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onConnectionReady(Tp::PendingOperation*)));
}

void TubeLauncher::onConnectionReady(Tp::PendingOperation* op)
{
    kDebug();

    // Connection is now ready. Lets get ContactPtrs for the contacts we want.
    QStringList identifiers;
    // FIXME: Hard coded contact identifiers
    identifiers << "telepathy-test2@kdetalk.net";

    Tp::PendingContacts *pc = m_connection->contactManager()->contactsForIdentifiers(identifiers);

    connect(pc, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onContactsGot(Tp::PendingOperation*)));
}

void TubeLauncher::onContactsGot(Tp::PendingOperation* op)
{
    kDebug();

    // Get the PendingContacts object.
    Tp::PendingContacts *pc = qobject_cast<Tp::PendingContacts*>(op);

    m_targetContacts = pc->contacts();
}

void TubeLauncher::onGotTubeChannel(Tp::ChannelPtr channel)
{
    kDebug();

    // FIXME: Invite some participants.
    m_channel = channel;
    Tp::Features features;
    features << Tp::Channel::FeatureCore << Tp::DBusTubeChannel::FeatureDBusTube << Tp::OutgoingDBusTubeChannel::FeatureDBusTube;

    connect(m_channel->becomeReady(features),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onChannelReady()));

    QTimer::singleShot(5000, this, SLOT(onChannelReady()));
}

void TubeLauncher::onChannelReady()
{
    kDebug();
   // m_channel->groupAddContacts(m_targetContacts);
}

void TubeLauncher::onStartWhiteBoardSessionClicked() {
    kDebug();

    // Make the contact list for people we will invite
    //QList<Tp::ContactPtr> invitees;

    // HACK: We will assume the account is ready.
    QVariantMap properties;
    properties.insert(TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType"),
                      QLatin1String(TP_QT_IFACE_CHANNEL_TYPE_DBUS_TUBE));
    properties.insert(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandleType"),
  //                    Tp::HandleTypeRoom);
                      Tp::HandleTypeContact);
    properties.insert(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetID"),
  //                    QLatin1String("omgwtfbbqoooooo"));
                      QLatin1String("telepathy-test2@kdetalk.net"));
    properties.insert(TP_QT_IFACE_CHANNEL_TYPE_DBUS_TUBE + QLatin1String(".ServiceName"),
                      QLatin1String("org.kde.KWhiteBoard"));

    m_account->createChannel(properties);
}


#include "TubeLauncher.moc"

