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
#include "projectile.h"
#include "lawnmower.h"

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
    void sortComboBox();
    void setButtonsCheckable(bool checkable);
    void setButtonsCheck(bool check);
    void saveFile(vector<User *> user_vector, User * current_user);

private slots:
    void uiUpdater();

    void drawPlantChecker();

    void drawZombieChecker();

    void sunDropper();

    void plantShooter();

    void plantItemChecker();

    void projectileChecker();

    void gameEndChecker();

    void levelEndChecker();

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

    void on_restartButton_clicked();


    void on_quitButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene * scene;
    GameManager * gameManager;
    vector<User *> users;
    vector<Level *> levels;
    vector<Plant *> plants;
    vector<Zombie *> zombies;
    vector<int> currentZombieSequence;
    vector<Plant*> existingPlants;
    vector<Zombie*> existingZombies;
    Plant * currentPlant;
    User * currentUser;
    Level * currentLevel;
    Zombie *currentZombie;
    int plantIndex;
    bool plantReady;
    QPoint lastClick;
    QPointF itemPos;
    QTimer * timer;
    QTimer * sceneTimer;
    QTimer * sunTimer;
    QTimer * uiTimer;
    QTimer * zombieTimer;
    QTimer * shootingTimer;
    QTimer * plantItemTimer;
    QTimer * projectileTimer;
    QTimer * gameEndTimer;
    QTimer * levelEndTimer;
    QElapsedTimer * sunDropTimer;
    int sunPoints;
    int sunTimeCounter;
    int zombieCounter;
    int zombieTimeCounter;
    double lastZombieSpawnTime;
    double zombieInterval;
    int rows;
    int columns;
    vector <vector<QPointF> > grid; // grid
    vector <Sun*> suns;
    vector <Sun*> plantSuns;
    vector <Projectile*> projectiles;
    vector <int> zombieCollisionCounters;
    vector <Lawnmower*> lawnmowers;
    bool levelEnd;
    vector <QGraphicsTextItem *> textItems;
};

#endif // MAINWINDOW_H
