#ifndef LAWNMOWER_H
#define LAWNMOWER_H

#include <QGraphicsPixmapItem>

class Lawnmower : public QGraphicsPixmapItem
{
private:
    double speed;
    bool ready;
public:
    Lawnmower();
    void advance(int phase);
    bool getReady() const;
    void setReady(bool value);
};

#endif // LAWNMOWER_H
