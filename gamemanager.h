#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "user.h"
#include "level.h"
#include "lawn.h"
#include <vector>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
//#include "ui_mainwindow.h"
//#include "mainwindow.h"
#include <QImage>
#include "plant.h"
#include "zombie.h"

using std::string;
using std::vector;

class GameManager
{
private:
    std::vector<User *> userVector;
    std::vector<Level *> levelVector;
    std::vector<Plant *> plantVector;
    std::vector<Zombie *> zombieVector;

    int mIndex;
    QFile mFile;
    QString mFileName, mPath;

public:
    //void readFile();
    void readPlayersFile(QString file_name);
    void readLevelsFile(QString file_name);
    void readPlantsFile(QString file_name);
    void readZombiesFile(QString file_name);
    void saveFile(vector<User *> user_vector, User * current_user);
    void retImage(QString image_path);
    void displayImage(QImage * image);
    vector<User *> getUserVector();
    vector<Level *> getLevelVector();
    vector<Plant *> getPlantVector();
    vector<Zombie *> getZombieVector();
    //void addUser();
    //void delUser();
    //void selUser();
};

#endif // GAMEMANAGER_H

