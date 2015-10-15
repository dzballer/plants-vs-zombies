#include "sun.h"
#include "mainwindow.h"
#include "lawn.h"


int Sun::getDuration() const
{
    return duration;
}

void Sun::setDuration(int value)
{
    duration = value;
}

void Sun::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
    deleteReady = true;
}

bool Sun::getDeleteReady() const
{
    return deleteReady;
}

void Sun::setDeleteReady(bool value)
{
    deleteReady = value;
}

QPointF Sun::getInitPos() const
{
    return initPos;
}

void Sun::setInitPos(const QPointF &value)
{
    initPos = value;
}

QPointF Sun::getFinalPos() const
{
    return finalPos;
}

void Sun::setFinalPos(const QPointF &value)
{
    finalPos = value;
}

Sun::Sun() : yVelocity(4), duration(0), deleteReady(false)
{
}

// Timer triggers advance() method of scene, which in turn calls advance() method of each object
void Sun::advance(int phase)
{
    // advance() method is called twice. Once with phase set to 0 indicating the object is about to advence
    // Second time with phase set to 1 for the actual advance.
    if(!phase) return;  // We don't do anything to prepare objects for advancing
    if(this->pos().y() >= finalPos.y()) return; // When sunpoint drops from sky onto square, it will stop moving.

    this->setPos(this->pos().x(), this->pos().y()+yVelocity);
}
