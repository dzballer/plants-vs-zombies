#include "sun.h"
#include "mainwindow.h"


int Sun::getDuration() const
{
    return duration;
}

void Sun::setDuration(int value)
{
    duration = value;
}

void Sun::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
    deleteReady =true;
    qDebug() << "clicked";

}

bool Sun::getDeleteReady() const
{
    return deleteReady;
}

void Sun::setDeleteReady(bool value)
{
    deleteReady = value;
}

/*int Sun::getSunTime()
{
    sunTimer->elapsed();
}

Sun::~Sun()
{
    delete sunTimer;
}*/
Sun::Sun() : yVelocity(10), duration(0), deleteReady(false)//, sunTimer(new QElapsedTimer)
{
    //sunTimer->start();
}

// Timer triggers advance() method of scene, which in turn calls advance() method of each object
void Sun::advance(int phase)
{
    // advance() method is called twice. Once with phase set to 0 indicating the object is about to advence
    // Second time with phase set to 1 for the actual advance.
    if(!phase) return;  // We don't do anything to prepare objects for advancing

    //this->setos().setY(this->pos().y() + yVelocity);
    this->setPos(this->pos().x(), this->pos().y()+yVelocity);
}

/*QRectF Sun::boundingRect()
{
    return QRectF(0,0, lawnWidth, lawnHeight);
}*/

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
