#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamemanager.h"
#include "lawn.h"
#include <QImage>
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QFileDialog>
#include <QRect>
#include <vector>
#include "plant.h"
#include <QMouseEvent>
#include <sun.h>
#include <QElapsedTimer>

using std::vector;

const int lawnLeft = 180, lawnRight = 700, lawnTop = 55, lawnBottom = 410;
const int lawnWidth = lawnRight-lawnLeft, lawnHeight = lawnBottom-lawnTop;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mousePressEvent(QMouseEvent * e);

private slots:
    void uiUpdater();

    void drawPlantChecker();

    void sunDropper();

    void on_p1Button_clicked();

    void on_p2Button_clicked();

    void on_p3Button_clicked();

    void on_p4Button_clicked();

    void on_p5Button_clicked();

    void on_p6Button_clicked();

    void on_p7Button_clicked();

    void on_p8Button_clicked();

    void on_userComboBox_currentIndexChanged(int index);

    void on_deleteButton_clicked();

    void on_newButton_clicked();

    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene * scene;
    GameManager * gameManager;
    vector<User *> users;
    vector<Level *> levels;
    vector<Plant *> plants;
    vector<QPointF> existingPlants; // Using to check if plants have already been drawn. Stores already existing plant coordinates.
    Plant * currentPlant;
    User * currentUser;
    int plantIndex;
    bool plantReady;
    QPoint lastClick;
    QPointF itemPos;
    QTimer * timer;
    QTimer * sunTimer;
    QTimer * uiTimer;
    QElapsedTimer * sunDropTimer;
    int sunPoints;
    int sunTimeCounter;
    int rows;
    int columns;
    vector <vector<QPointF> > grid; // grid
    vector <Sun*> suns;
};

#endif // MAINWINDOW_H
