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
    int duration;
    bool deleteReady;
    QPointF initPos;
    QPointF finalPos;

public:
    Sun();
    void advance(int phase);
    int getDuration() const;
    void setDuration(int value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    bool getDeleteReady() const;
    void setDeleteReady(bool value);
    QPointF getInitPos() const;
    void setInitPos(const QPointF &value);
    QPointF getFinalPos() const;
    void setFinalPos(const QPointF &value);
};

#endif // SUN_H
