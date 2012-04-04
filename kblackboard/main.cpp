// KDE
#include <KCmdLineArgs>
#include <KApplication>
#include <KAboutData>
#include <KLocale>

#include "telepathyblackboard.h"

#include <TelepathyQt/AccountFactory>
#include <TelepathyQt/ClientRegistrar>
#include <TelepathyQt/ConnectionFactory>
#include <TelepathyQt/StreamTubeChannel>
#include <TelepathyQt/Debug>

int main(int argc, char *argv[])
{
    KAboutData aboutData("telepathy-whiteboard", 0, ki18n("telepathy-whiteboard"),
                         "0.1", ki18n("Description here"),
                         KAboutData::License_GPL,
                         ki18n("(c) KDE"),
                         KLocalizedString(), "", "kde-devel@kde.org");

    aboutData.addAuthor(ki18n("David Edmundson"), ki18n("Author"), "kde@davidedmundson.co.uk");

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("debug", ki18n("Show telepathy debugging information"));

//    options.add("server", ki18n("is server"));
    KCmdLineArgs::addCmdLineOptions(options);
    KCmdLineArgs::parsedArgs();

    Tp::registerTypes();

    KApplication app;

    //Enable telepathy-Qt4 debug
    Tp::enableDebug(KCmdLineArgs::parsedArgs()->isSet("debug"));
    Tp::enableWarnings(true);

    Tp::AccountFactoryPtr accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus());

    Tp::ConnectionFactoryPtr  connectionFactory = Tp::ConnectionFactory::create(QDBusConnection::sessionBus());

    Tp::ChannelFactoryPtr channelFactory = Tp::ChannelFactory::create(QDBusConnection::sessionBus());
    channelFactory->addCommonFeatures(Tp::Channel::FeatureCore);
    channelFactory->addFeaturesForIncomingStreamTubes(Tp::StreamTubeChannel::FeatureCore);
    channelFactory->addFeaturesForOutgoingStreamTubes(Tp::StreamTubeChannel::FeatureCore);

    Tp::ContactFactoryPtr contactFactory = Tp::ContactFactory::create();

    Tp::ClientRegistrarPtr registrar = Tp::ClientRegistrar::create(accountFactory,
                                                                   connectionFactory,
                                                                   channelFactory,
                                                                   contactFactory);

    Tp::SharedPtr<TelepathyBlackboard> handler = Tp::SharedPtr<TelepathyBlackboard>(
                new TelepathyBlackboard());
    registrar->registerClient(Tp::AbstractClientPtr(handler),
                              QLatin1String("KDE.Whiteboard"));



    return app.exec();
}

