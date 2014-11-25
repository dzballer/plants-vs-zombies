#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <QImage>

using std::string;

class Plant
{
private:
    int index;
    string name;
    int cost;
    int life;
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
public:
    Plant();
    void setIndex(int an_index);
    void setName(string a_name);
    void setCost(int num_cost);
    void setLife(int num_life);
    void setRange(int num_range);
    void setDamage(int num_damage);
    void setFireRate(int fire_rate);
    void setSplash(int num_splash);
    void setSlow(int num_slow);
    void setBomb(int num_bomb);
    void setSeeding(int seeding_time);
    void setSun(int num_sun);
    void setNeed(int num_need);
    string getName();
    int getCost();
    int getLife();
    int getRange();
    int getDamage();
    double getFireRate();
    int getSplash();
    int getSlow();
    int getBomb();
    double getSeeding();
    int getSun();
    int getNeed();
};

#endif // PLANT_H
