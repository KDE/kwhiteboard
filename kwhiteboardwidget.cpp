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

#include "kwhiteboardwidget.h"
#include <KDebug>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QDBusConnection>

static const char* s_objectPath = "/whiteboard";
static const char* s_dbusInterface = "org.kde.KWhiteBoard";

KWhiteBoardWidget::KWhiteBoardWidget(QWidget* parent)
    : QWidget(parent)
{
    //new KWhiteBoardIface(this); // our "adaptor"

    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerObject(QString::fromLatin1(s_objectPath), this, QDBusConnection::ExportAllSignals | QDBusConnection::ExportAllSlots)) {
        kWarning() << "Could not register object to D-BUS!";
        qApp->exit(1);
    }
    dbus.connect(QString(), s_objectPath, s_dbusInterface, "sigDrawLine", this, SLOT(drawLine(int, int, int, int)));

}

void KWhiteBoardWidget::drawLine(int x1, int y1, int x2, int y2)
{
    QPainter p(&m_pixmap);
    p.drawLine(x1, y1, x2, y2);
    update();
}

void KWhiteBoardWidget::resizeEvent(QResizeEvent *)
{
    // Never shrink the buffer pixmap, to avoid losing data.
    QPixmap tmp(size().expandedTo(m_pixmap.size()));
    tmp.fill(Qt::white);
    QPainter painter(&tmp);
    painter.drawPixmap(0, 0, m_pixmap);
    painter.end();
    m_pixmap = tmp;
}

void KWhiteBoardWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, m_pixmap);
}

void KWhiteBoardWidget::mousePressEvent(QMouseEvent *event)
{
    m_startPoint = event->pos();
}

void KWhiteBoardWidget::mouseMoveEvent(QMouseEvent *event)
{
    emit sigDrawLine(m_startPoint.x(), m_startPoint.y(), event->x(), event->y());
    m_startPoint = event->pos();
}

#include "kwhiteboardwidget.moc"
