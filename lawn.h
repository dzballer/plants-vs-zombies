#ifndef LAWN_H
#define LAWN_H

#include <QGraphicsView>
#include <QDebug>
#include <QMouseEvent>

class Lawn : public QGraphicsView
{
    Q_OBJECT
private:
    QPoint pos;
    bool ready; // true when graphicsView is clicked
public:
    explicit Lawn(QWidget *parent = 0);

    bool getReady() const;
    void setReady(bool value);

signals:

public slots:
    void mousePressEvent(QMouseEvent *e);
    QPoint getPos();

};

#endif // LAWN_H
