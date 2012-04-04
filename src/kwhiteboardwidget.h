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

#ifndef KWHITEBOARDWIDGET_H
#define KWHITEBOARDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QDBusConnection>

class KWhiteboardWidget : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KWhiteboard")

public:
    KWhiteboardWidget(QWidget* parent, const QDBusConnection &conn);

public Q_SLOTS:
    void drawLine(int x1, int y1, int x2, int y2);

Q_SIGNALS:
    void sigDrawLine(int x1, int y1, int x2, int y2);

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

private:
    QPixmap m_pixmap;
    QPoint m_startPoint;
};

#endif
