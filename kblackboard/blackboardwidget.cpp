#include "blackboardwidget.h"

#include <QTcpSocket>
#include <QMouseEvent>
#include <QPainter>


BlackboardWidget::BlackboardWidget(QWidget *parent) :
    QWidget(parent),
    m_socket(0)
{
}

void BlackboardWidget::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
    connect(m_socket, SIGNAL(readyRead()), SLOT(onDataAvailable()));
    qDebug() << "connected";
}

void BlackboardWidget::onDataAvailable()
{
    QPoint start, end;

    QDataStream ds(m_socket);
    while(m_socket->bytesAvailable() > static_cast<int>(sizeof(qint32) *4)) {
        ds >> start >> end;
        drawLine(start, end);
    }
}

void BlackboardWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawPixmap(0, 0, m_pixmap);
}

void BlackboardWidget::mousePressEvent(QMouseEvent *e)
{
    m_startPoint = e->pos();
}


void BlackboardWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint endPoint = e->pos();

    QDataStream ds(m_socket);


    if(m_socket) {
        ds << m_startPoint << endPoint;
    }

    drawLine(m_startPoint, endPoint);

    m_startPoint = e->pos();
}

void BlackboardWidget::drawLine(const QPoint &start, const QPoint &end)
{
    QPainter painter(&m_pixmap);
    QPen pen = painter.pen();
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(start, end);

    //optimisation stage only repaint the relevant area.
    update();
}

void BlackboardWidget::resizeEvent(QResizeEvent *)
{
    // Never shrink the buffer pixmap, to avoid losing data.
    //(not that that matters...it all gets whited out anyway.)
    QPixmap tmp(size().expandedTo(m_pixmap.size()));
    tmp.fill(Qt::white);
    QPainter painter(&tmp);
    painter.drawPixmap(0, 0, m_pixmap);
    painter.end();
    m_pixmap = tmp;
}

