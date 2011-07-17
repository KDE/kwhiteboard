#ifndef WHITEBOARDWIDGET_H
#define WHITEBOARDWIDGET_H

#include <QWidget>

class QTcpSocket;

class WhiteboardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WhiteboardWidget(QWidget *parent = 0);
    void setSocket(QTcpSocket* socket);

protected:

    void drawLine(const QPoint &start, const QPoint &end);

    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

private slots:
    void onDataAvailable();

private:
    QTcpSocket* m_socket;
    QPixmap m_pixmap;
    QPoint m_startPoint;

};

#endif // WHITEBOARDWIDGET_H
