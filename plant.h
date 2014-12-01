#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <vector>
#include <zombie.h>
#include <QProgressBar>

using std::string;

using std::vector;

class Plant : public QGraphicsPixmapItem
{
private:
    int index;
    string name;
    int cost;
    double life;
    int range;
    int damage;
    double fireRate;
    int splash; // For splash = n, zombies in an nxn area around the plant receive damage.
    int slow;
    int bomb;
    double seeding;
    int sun;
    int need;
    QImage * mImage;
    int shootingTimeCounter;
    bool alive;
    bool collide;
    vector <Zombie *> collideZombies; // Zombies that plant is currently colliding with
    QGraphicsTextItem * lifeLabel;
public:
    Plant();
    Plant(Plant * plant);
    void setIndex(int an_index);
    void setName(string a_name);
    void setCost(int num_cost);
    void setLife(double num_life);
    void setRange(int num_range);
    void setDamage(int num_damage);
    void setFireRate(int fire_rate);
    void setSplash(int num_splash);
    void setSlow(int num_slow);
    void setBomb(int num_bomb);
    void setSeeding(int seeding_time);
    void setSun(int num_sun);
    void setNeed(int num_need);
    int getIndex();
    string getName();
    int getCost();
    double getLife();
    int getRange();
    int getDamage();
    double getFireRate();
    int getSplash();
    int getSlow();
    int getBomb();
    double getSeeding();
    int getSun();
    int getNeed();
    int getShootingTimeCounter() const;
    void setShootingTimeCounter(int value);
    bool getAlive() const;
    void setAlive(bool value);
    vector<Zombie *> getCollideZombies() const;
    void setCollideZombies(const vector<Zombie *> &value);
    void addCollideZombie(Zombie * zombie);
    bool getCollide() const;
    void setCollide(bool value);
    QProgressBar *getHpBar() const;
    void setHpBar(QProgressBar *value);
    QGraphicsTextItem *getLifeLabel() const;
    void setLifeLabel(QGraphicsTextItem *value);
};

#endif // PLANT_H
