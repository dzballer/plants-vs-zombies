#include "zombie.h"
#include <QDebug>

string Zombie::getName() const
{
    return name;
}

void Zombie::setName(const string &value)
{
    name = value;
}

int Zombie::getArmor() const
{
    return armor;
}

void Zombie::setArmor(int value)
{
    armor = value;
}

int Zombie::getLife() const
{
    return life;
}

void Zombie::setLife(int value)
{
    life = value;
}

int Zombie::getAttack() const
{
    return attack;
}

void Zombie::setAttack(int value)
{
    attack = value;
}

double Zombie::getAttackRate() const
{
    return attackRate;
}

void Zombie::setAttackRate(double value)
{
    attackRate = value;
}

double Zombie::getSpeed() const
{
    return speed;
}

void Zombie::setSpeed(double value)
{
    speed = value;
}

int Zombie::getIndex() const
{
    return index;
}

void Zombie::setIndex(int value)
{
    index = value;
}

void Zombie::advance(int phase)
{
    // advance() method is called twice. Once with phase set to 0 indicating the object is about to advence
    // Second time with phase set to 1 for the actual advance.
    if(!phase) return;  // We don't do anything to prepare objects for advancing

    // If zombie is dead, take it out of screen
    if(!alive)
    {
        this->setPos(2000,this->pos().y());
        lifeLabel->setPos(2000,lifeLabel->pos().y());
        return;
    }

    if(collide) return; // If zombie is colliding with "*other", then zombie doesn't move.

    this->setPos(this->pos().x()-speed,this->pos().y());
    //this->lifeLabel->setPos(this->pos().x()+20,this->pos().y()-10); // why crashing??
}

/*bool Zombie::collidesWithItem(const QGraphicsItem *other) const
{
    collide = true;
    return true;
}

bool Zombie::isCollidingWith(QGraphicsItem *other)
{
    if(this->collidesWithItem(other))
    {
        collide = true;
        return true;
    }
    else
    {
        collide = false;
        return false;
    }
}*/

bool Zombie::getCollide() const
{
    return collide;
}

void Zombie::setCollide(bool value)
{
    collide = value;
}

void Zombie::startCollisionTimer()
{
    collisionTimer = new QElapsedTimer;
    collisionTimer->start();
    timerStarted = true;
}

/*void Zombie::stopCollisionTimer()
{
    collisionTimer = new QElapsedTimer;
    collisionTimer->stop();
    timerStarted = false;
}*/

int Zombie::getCollisionTime()
{
    return collisionTimer->elapsed();
}


bool Zombie::isTimerStarted() const
{
    return timerStarted;
}

void Zombie::setTimerStarted(bool value)
{
    timerStarted = value;
}

bool Zombie::getAlive() const
{
    return alive;
}

void Zombie::setAlive(bool value)
{
    alive = value;
}

QGraphicsTextItem *Zombie::getLifeLabel() const
{
    return lifeLabel;
}

void Zombie::setLifeLabel(QGraphicsTextItem *value)
{
    lifeLabel = value;
}
Zombie::Zombie() : collide(false), timerStarted(false), alive(true)
{

}

Zombie::Zombie(Zombie *zombie) : collide(false), timerStarted(false), alive(true)
{
    this->setIndex(zombie->getIndex());
    this->setName(zombie->getName());
    this->setArmor(zombie->getArmor());
    this->setLife(zombie->getLife());
    this->setAttack(zombie->getAttack());
    this->setAttackRate(zombie->getAttackRate());
    this->setSpeed(zombie->getSpeed());

    //lifeLabel->setPos(this->pos().x()+20,this->pos().y()-10);

}
