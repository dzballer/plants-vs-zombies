#include "projectile.h"

Projectile::Projectile() : speed(5)
{
}

void Projectile::advance(int phase)
{
    // advance() method is called twice. Once with phase set to 0 indicating the object is about to advence
    // Second time with phase set to 1 for the actual advance.
    if(!phase) return;  // We don't do anything to prepare objects for advancing

    //this->setos().setY(this->pos().y() + yVelocity);
    this->setPos(this->pos().x() + speed, this->pos().y());
}
