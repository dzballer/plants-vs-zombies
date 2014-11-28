#ifndef SUN_H
#define SUN_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QGraphicsView>

class Sun : public QGraphicsPixmapItem
{
private:
    double yVelocity; // xVelocity should always be 0 so no need in making a member
    int duration; //
    bool deleteReady;
    //QElapsedTimer * sunTimer;
public:
    Sun();
    void move(int phase);
    //QRectF boundingRect();
    //void setPos(double x, double y);
    int getDuration() const;
    void setDuration(int value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    bool getDeleteReady() const;
    void setDeleteReady(bool value);
    //int getSunTime();
    //~Sun();
};

#endif // SUN_H

/*class Animal : public QGraphicsItem
{
private:

protected:
    double mVx, mVy; // x and y velocities of animal
    Body * body;
    enum {W=20}; // diameter of object

public:
    Animal();
    void setBody(Body);
    void move(double);
    virtual void draw() = 0;
    virtual ~Animal(){};
    void advance(int phase);
};

Animal::Animal()
{
    body = new Body;

    // sets the position of the animal randomly
    setPos(rand()%(MainWindow::WIDTH-W),rand()%(MainWindow::HEIGHT-W));
}

void Animal::setBody(Body b)
{
    *body = b;
}


void Animal::move(double time)
{
    body->move(time);
}

// Timer triggers advance() method of scene, which in turn calls advance() method of each object
void Animal::advance(int phase)
{
    // advance() method is called twice. Once with phase set to 0 indicating the object is about to advence
    // Second time with phase set to 1 for the actual advance.
    if(!phase) return;  // We don't do anything to prepare objects for advancing

    QPointF p = this->pos();    // current position
    // qDebug() << p;

    // Check boundaries (0,0)-(WIDTH,HEIGHT)
    // Setting xCenter and yCenter to the center of each animal's bounding rect position
    double xCenter = p.x() + W/2, yCenter = p.y() + W/2;
    if (xCenter < W/2 || xCenter > MainWindow::WIDTH - W/2) // reverse x-direction
        mVx *= -1;
    if (yCenter < W/2 || yCenter > MainWindow::HEIGHT - W/2) // reverse y-direction
        mVy *= -1;

    setPos(p.x()+mVx, p.y()+mVy); // move based on velocity
}
*/
