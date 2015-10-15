#include "projectile.h"


bool Projectile::getSlow() const
{
    return slow;
}

void Projectile::setSlow(bool value)
{
    slow = value;
}
Projectile::Projectile() : speed(5), slow(false)
{
}

void Projectile::advance(int phase)
{
    // advance() method is called twice. Once with phase set to 0 indicating the object is about to advence
    // Second time with phase set to 1 for the actual advance.
    if(!phase) return;  // We don't do anything to prepare objects for advancing

    this->setPos(this->pos().x() + speed, this->pos().y());
}
