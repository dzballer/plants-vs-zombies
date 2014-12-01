#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Projectile : public QGraphicsPixmapItem
{
private:
    double speed;
    bool slow;
    //QTimer * projectileTimer;
public:
    Projectile();
    void advance(int phase);
    bool getSlow() const;
    void setSlow(bool value);
};

#endif // PROJECTILE_H
