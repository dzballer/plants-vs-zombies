#include "plant.h"

Plant::Plant()
{
}

int Plant::setIndex(int an_index)
{
    index = an_index;
}

string Plant::setName(string a_name)
{
    name = a_name;
}

int Plant::setCost(int num_cost)
{
    cost = num_cost;
}

int Plant::setLife(int num_life)
{
    life = num_life;
}

int Plant::setRange(int num_range)
{
    range = num_range;
}

int Plant::setDamage(int num_damage)
{
    damage = num_damage;
}

double Plant::setFireRate(int fire_rate)
{
    fireRate = fire_rate;
}

int Plant::setSplash(int num_splash)
{
    splash = num_splash;
}

int Plant::setSlow(int num_slow)
{
    slow = num_slow;
}

int Plant::setBomb(int num_bomb)
{
    bomb = num_bomb;
}

double Plant::setSeeding(int seeding_time)
{
    seeding = seeding_time;
}

int Plant::setSun(int num_sun)
{
    sun = num_sun;
}

int Plant::setNeed(int num_need)
{
    need = num_need;
}
