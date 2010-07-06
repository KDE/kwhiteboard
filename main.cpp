/*
 *  Copyright (C) 2010 David Faure   <faure@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation;
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include <KAboutData>
#include <KApplication>
#include <KCmdLineArgs>
#include <kwhiteboard.h>

int main(int argc, char *argv[])
{
    KAboutData aboutData("kwhiteboard", 0, ki18n("KWhiteBoard"),
                         "1.0", ki18n("KDE WhiteBoard Application"), KAboutData::License_LGPL);
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication app;

    KWhiteBoard *mainWindow = new KWhiteBoard();
    mainWindow->show();

    return app.exec();
}
