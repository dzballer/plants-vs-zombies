#include "lawn.h"


bool Lawn::getReady() const
{
    return ready;
}

void Lawn::setReady(bool value)
{
    ready = value;
}
Lawn::Lawn(QWidget *parent) :
    QGraphicsView(parent)
{

}

void Lawn::mousePressEvent(QMouseEvent *e)
{
    pos = e->pos();
    //qDebug() << e->pos();
    ready = true;
    //qDebug() << ready << "1";
    QGraphicsView::mousePressEvent(e);
    //qDebug() << ready << "2";
}

QPoint Lawn::getPos()
{
    return pos;
}
