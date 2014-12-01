#include "plant.h"


int Plant::getShootingTimeCounter() const
{
    return shootingTimeCounter;
}

void Plant::setShootingTimeCounter(int value)
{
    shootingTimeCounter = value;
}

bool Plant::getAlive() const
{
    return alive;
}

void Plant::setAlive(bool value)
{
    alive = value;
}

vector<Zombie *> Plant::getCollideZombies() const
{
    return collideZombies;
}

void Plant::setCollideZombies(const vector<Zombie *> &value)
{
    collideZombies = value;
}

void Plant::addCollideZombie(Zombie *zombie)
{
    collideZombies.push_back(zombie);
}


bool Plant::getCollide() const
{
    return collide;
}

void Plant::setCollide(bool value)
{
    collide = value;
}


QGraphicsTextItem *Plant::getLifeLabel() const
{
    return lifeLabel;
}

void Plant::setLifeLabel(QGraphicsTextItem *value)
{
    lifeLabel = value;
}

bool Plant::getChompReady() const
{
    return chompReady;
}

void Plant::setChompReady(bool value)
{
    chompReady = value;
}
Plant::Plant() : shootingTimeCounter(0), alive(true), lifeLabel(new QGraphicsTextItem)
{
}

Plant::Plant(Plant *plant) : shootingTimeCounter(0), alive(true), chompReady(false), lifeLabel(new QGraphicsTextItem)
{
    this->index = (plant->getIndex());
    this->name=(plant->getName());
    this->cost=(plant->getCost());
    this->life=(plant->getLife());
    this->range=(plant->getRange());
    this->damage=(plant->getDamage());
    this->fireRate=(plant->getFireRate());
    this->splash=(plant->getSplash());
    this->slow=(plant->getSlow());
    this->bomb=(plant->getBomb());
    this->seeding=(plant->getSeeding());
    this->sun=(plant->getSun());
    this->need=(plant->getNeed());

}

void Plant::setIndex(int an_index)
{
    index = an_index;
}

void Plant::setName(string a_name)
{
    name = a_name;
}

void Plant::setCost(int num_cost)
{
    cost = num_cost;
}

void Plant::setLife(double num_life)
{
    life = num_life;
}

void Plant::setRange(int num_range)
{
    range = num_range;
}

void Plant::setDamage(int num_damage)
{
    damage = num_damage;
}

void Plant::setFireRate(int fire_rate)
{
    fireRate = fire_rate;
}

void Plant::setSplash(int num_splash)
{
    splash = num_splash;
}

void Plant::setSlow(int num_slow)
{
    slow = num_slow;
}

void Plant::setBomb(int num_bomb)
{
    bomb = num_bomb;
}

void Plant::setSeeding(int seeding_time)
{
    seeding = seeding_time;
}

void Plant::setSun(int num_sun)
{
    sun = num_sun;
}

void Plant::setNeed(int num_need)
{
    need = num_need;
}

int Plant::getIndex()
{
    return index;
}

string Plant::getName()
{
    return name;
}

int Plant::getCost()
{
    return cost;
}

double Plant::getLife()
{
    return life;
}

int Plant::getRange()
{
    return range;
}

int Plant::getDamage()
{
    return damage;
}

double Plant::getFireRate()
{
    return fireRate;
}

int Plant::getSplash()
{
    return splash;
}

int Plant::getSlow()
{
    return slow;
}

int Plant::getBomb()
{
    return bomb;
}

double Plant::getSeeding()
{
    return seeding;
}

int Plant::getSun()
{
    return sun;
}

int Plant::getNeed()
{
    return need;
}
