#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Projectile : public QGraphicsPixmapItem
{
private:
    double speed;
    QTimer * projectileTimer;
public:
    Projectile();
    void advance(int phase);
};

#endif // PROJECTILE_H
