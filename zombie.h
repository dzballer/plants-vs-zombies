#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <iostream>
#include <QGraphicsPixmapItem>
using std::string;

class Zombie : public QGraphicsPixmapItem
{
private:
    int index;
    string name;
    int armor;
    int life;
    int attack;
    double attackRate;
    double speed;

public:
    Zombie();
    string getName() const;
    void setName(const string &value);
    int getArmor() const;
    void setArmor(int value);
    int getLife() const;
    void setLife(int value);
    int getAttack() const;
    void setAttack(int value);
    double getAttackRate() const;
    void setAttackRate(double value);
    double getSpeed() const;
    void setSpeed(double value);
    int getIndex() const;
    void setIndex(int value);
    void advance(int phase);
};

#endif // ZOMBIE_H
