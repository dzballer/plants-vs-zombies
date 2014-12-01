#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <iostream>
#include <QGraphicsPixmapItem>
#include <QElapsedTimer>
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
    bool collide;
    bool timerStarted;
    QElapsedTimer * collisionTimer;
    bool alive;
    bool slowed;
    QGraphicsTextItem * lifeLabel;
public:
    Zombie();
    Zombie(Zombie *zombie);
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
    //bool collidesWithItem(const QGraphicsItem *other) const;
    //bool isCollidingWith(QGraphicsItem * other);
    bool getCollide() const;
    void setCollide(bool value);
    void startCollisionTimer();
    //void stopCollisionTimer();
    int getCollisionTime();
    bool isTimerStarted() const;
    void setTimerStarted(bool value);
    bool getAlive() const;
    void setAlive(bool value);
    QGraphicsTextItem *getLifeLabel() const;
    void setLifeLabel(QGraphicsTextItem *value);
    bool getSlowed() const;
    void setSlowed(bool value);
};

#endif // ZOMBIE_H
