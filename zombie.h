#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <iostream>

using std::string;

class Zombie
{
private:
    string name;
    int armor;
    int life;
    int attack;
    double attackRate;
    double speed;

public:
    Zombie();
};

#endif // ZOMBIE_H
