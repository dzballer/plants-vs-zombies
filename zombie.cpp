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
    if(collide) return; // If zombie is colliding with "*other", then zombie doesn't move.
    this->setPos(this->pos().x()-speed,this->pos().y());
    //this->pos().setX(this->pos().x() - speed);
    //qDebug() << this->pos().x()-speed;
    //this->setPos();
    //qDebug() << "advancing";
}

/*bool Zombie::collidesWithItem(const QGraphicsItem *other) const
{
    collide = true;
    return true;
}/*

/*bool Zombie::isCollidingWith(QGraphicsItem *other)
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
Zombie::Zombie() : collide(false), timerStarted(false)
{

}

Zombie::Zombie(Zombie *zombie) : collide(false), timerStarted(false)
{
    this->setIndex(zombie->getIndex());
    this->setName(zombie->getName());
    this->setArmor(zombie->getArmor());
    this->setLife(zombie->getLife());
    this->setAttack(zombie->getAttack());
    this->setAttackRate(zombie->getAttackRate());
    this->setSpeed(zombie->getSpeed());
}
