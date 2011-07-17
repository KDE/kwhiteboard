// KDE
#include <KCmdLineArgs>
#include <KApplication>
#include <KAboutData>
#include <KLocale>

#include "telepathywhiteboard.h"

#include <TelepathyQt4/AccountFactory>
#include <TelepathyQt4/ClientRegistrar>
#include <TelepathyQt4/ConnectionFactory>
#include <TelepathyQt4/StreamTubeChannel>
#include <TelepathyQt4/Debug>

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
    //Enable telepathy-Qt4 debug
    Tp::enableDebug(KCmdLineArgs::parsedArgs()->isSet("debug"));
    Tp::enableWarnings(true);

    Tp::AccountFactoryPtr accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus());

    Tp::ConnectionFactoryPtr  connectionFactory = Tp::ConnectionFactory::create(QDBusConnection::sessionBus());

    Tp::ChannelFactoryPtr channelFactory = Tp::ChannelFactory::create(QDBusConnection::sessionBus());
    channelFactory->addCommonFeatures(Tp::Channel::FeatureCore);
    channelFactory->addFeaturesForIncomingStreamTubes(Tp::StreamTubeChannel::FeatureStreamTube);
    channelFactory->addFeaturesForOutgoingStreamTubes(Tp::StreamTubeChannel::FeatureStreamTube);

    Tp::ContactFactoryPtr contactFactory = Tp::ContactFactory::create();

    Tp::ClientRegistrarPtr registrar = Tp::ClientRegistrar::create(accountFactory,
                                                                   connectionFactory,
                                                                   channelFactory,
                                                                   contactFactory);

    Tp::SharedPtr<TelepathyWhiteboard> handler = Tp::SharedPtr<TelepathyWhiteboard>(
                new TelepathyWhiteboard());
    registrar->registerClient(Tp::AbstractClientPtr(handler),
                              QLatin1String("KDE.Whiteboard"));



    KApplication app;

    return app.exec();
}

