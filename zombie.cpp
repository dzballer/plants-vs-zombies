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

    this->setPos(this->pos().x()-speed/10,this->pos().y());
    //this->pos().setX(this->pos().x() - speed);
    //qDebug() << this->pos().x()-speed;
    //this->setPos();
    //qDebug() << "advancing";
}
Zombie::Zombie()
{
}
