#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <QImage>

using std::string;

class Plant
{
private:
    string name;
    int cost;
    int life;
    int range;
    int damage;
    double fireRate;
    int splash; // For splash = n, zombies in an nxn area around the plant receive damage.
    bool slow;
    bool bomb;
    double seeding;
    bool sun;
    bool need;
    QImage * mImage;
public:
    Plant();
};

#endif // PLANT_H
