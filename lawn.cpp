#include "lawn.h"


Lawn::Lawn(QWidget *parent) :
    QGraphicsView(parent)
{

}

void Lawn::mousePressEvent(QMouseEvent *e)
{
    qDebug() << e->pos();
}
