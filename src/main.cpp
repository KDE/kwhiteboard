/*
 * From t-i-d main.cpp
 *
 * Copyright (C) 2009-2010 Collabora Ltd. <info@collabora.co.uk>
 *   @author George Goldberg <george.goldberg@collabora.co.uk>
 * Copyright (C) 2010 David Faure <faure@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

extern "C"
{
#include <signal.h>
}

#include "TelepathyChannelManager.h"
#include "TubeLauncher.h"
#include "kwhiteboard.h"

#include <KAboutData>
#include <KCmdLineArgs>
#include <KDebug>
#include <KApplication>

#include <QtCore/QTimer>

#include <TelepathyQt/Types>

namespace
{
    static void signal_handler(int signal)
    {
        if ((signal == SIGTERM) || (signal == SIGINT)) {
            QCoreApplication * const app(QCoreApplication::instance());
            if (app != 0) {
                app->quit();
            }
        }
    }
}

int main(int argc, char *argv[])
{
    KAboutData aboutData("kwhiteboard", 0, ki18n("KWhiteBoard"),
                         "1.0", ki18n("KDE WhiteBoard Application"), KAboutData::License_LGPL);
    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("initiate", ki18n("initiate"), "1");
    
    KCmdLineArgs::addCmdLineOptions(options);
    
    KApplication app;

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    
    
    // Initialise Telepathy.
    Tp::registerTypes();

    // Set up signal handlers.
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        kWarning() << "Setting up SIGINT signal handler failed.";
    }

    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        kWarning() << "Setting up SIGTERM signal handler failed.";
    }

    TelepathyChannelManager *tpChannelManager = new TelepathyChannelManager;
    tpChannelManager->initialize();

    // Set up the main widget
    //MainWidget *mainWidget = new MainWidget;
    

    //QObject::connect(mainWidget, SIGNAL(destroyed()),
      //      &app, SLOT(quit()));
    
    KWhiteBoard *mainWindow = new KWhiteBoard();
    mainWindow->show();

    if (args->isSet("initiate")) {

        TubeLauncher *tubeLauncher = new TubeLauncher;
        QObject::connect(tpChannelManager, SIGNAL(gotTubeChannel(Tp::ChannelPtr)),
                            tubeLauncher, SLOT(onGotTubeChannel(Tp::ChannelPtr)));

        QTimer::singleShot(10000, tubeLauncher, SLOT(onStartWhiteBoardSessionClicked()));
        

    }
    QObject::connect(tpChannelManager, SIGNAL(gotTubeDBusConnection(QDBusConnection)),
                     mainWindow, SLOT(onGotTubeDBusConnection(QDBusConnection)));

    kDebug() << "Let's go...";
    
    // Start event loop.
    return app.exec();
}

