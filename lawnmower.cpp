#include "lawnmower.h"


bool Lawnmower::getReady() const
{
    return ready;
}

void Lawnmower::setReady(bool value)
{
    ready = value;
}
Lawnmower::Lawnmower() : speed(10), ready(false)
{
}

void Lawnmower::advance(int phase)
{
    // advance() method is called twice. Once with phase set to 0 indicating the object is about to advence
    // Second time with phase set to 1 for the actual advance.
    if(!phase) return;  // We don't do anything to prepare objects for advancing

    if(!ready) return;
    this->setPos(this->pos().x() + speed, this->pos().y());
}
