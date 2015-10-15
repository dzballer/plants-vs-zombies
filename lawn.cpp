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
    ready = true;
    QGraphicsView::mousePressEvent(e);
}

QPoint Lawn::getPos()
{
    return pos;
}
