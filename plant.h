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
    int setIndex(int an_index);
    string setName(string a_name);
    int setCost(int num_cost);
    int setLife(int num_life);
    int setRange(int num_range);
    int setDamage(int num_damage);
    double setFireRate(int fire_rate);
    int setSplash(int num_splash);
    int setSlow(int num_slow);
    int setBomb(int num_bomb);
    double setSeeding(int seeding_time);
    int setSun(int num_sun);
    int setNeed(int num_need);
};

#endif // PLANT_H
