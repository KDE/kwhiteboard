/*
 * Copyright (C) 2009-2010 Collabora Ltd. <info@collabora.co.uk>
 *   @author George Goldberg <george.goldberg@collabora.co.uk>
 * Copyright (C) 2010 David Faure <faure@kde.org>
 * Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#include "kwhiteboard-handler.h"

#include <KAboutData>
#include <KCmdLineArgs>
#include <KDebug>

#include <TelepathyQt/Debug>
#include <TelepathyQt/ClientRegistrar>

#include <KTp/telepathy-handler-application.h>


int main(int argc, char *argv[])
{
    KAboutData aboutData("kwhiteboard", 0, ki18n("KWhiteboard"),
                         "1.0", ki18n("KDE Whiteboard Application"), KAboutData::License_LGPL);
    aboutData.addAuthor(ki18n("Daniele E. Domenichelli"), ki18n("Developer"), "daniele.domenichelli@gmail.com");
    KCmdLineArgs::init(argc, argv, &aboutData);

    KTp::TelepathyHandlerApplication app;

    Tp::SharedPtr<KWhiteboardHandler> kwhiteboardHandler (new KWhiteboardHandler(&app));

    // Set up the Telepathy Client Registrar.
    // TODO use Tp-Qt factories
    Tp::ClientRegistrarPtr registrar = Tp::ClientRegistrar::create();
    if (!registrar->registerClient(Tp::AbstractClientPtr(kwhiteboardHandler), "KTp.KWhiteboard")) {
        kDebug() << "KWhiteBoard already running. Exiting";
        return 1;
    }

    kDebug() << "Let's go...";

    // Start event loop.
    return app.exec();
}

