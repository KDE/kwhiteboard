/*
 * KWhiteboard/KBlackboard Main
 *
 * Copyright (C) 2011 David Edmundson <kde@davidedmundson.co.uk>
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

#include <KCmdLineArgs>
#include <KApplication>
#include <KAboutData>
#include <KLocale>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    KAboutData aboutData("kboard-launcher", 0,
                         ki18n("KWhiteboard/KBlackboard Launcher"),
                         "0.0.1");
    aboutData.addAuthor(ki18n("Daniele E. Domenichelli"), ki18n("Author"), "daniele.domenichelli@gmail.com");

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    KApplication app;

    MainWindow *w = new MainWindow();
    w->show();
    return app.exec();
}

