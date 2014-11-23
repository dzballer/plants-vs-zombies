#include "lawn.h"
#include <QDebug>
#include <QMouseEvent>

Lawn::Lawn(QWidget *parent) :
    QGraphicsView(parent)
{

}

void Lawn::mousePressEvent(QMouseEvent *e)
{
    qDebug() << e->pos();
}
