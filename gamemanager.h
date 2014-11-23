#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "user.h"
#include "level.h"
#include <vector>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include "ui_mainwindow.h"
//#include "mainwindow.h"
#include "lawn.h"
#include <QImage>

using std::string;

class GameManager
{
private:
    std::vector<User *> userVector;
    std::vector<Level *> levelVector;

    int mIndex;
    QFile mFile;
    QString mFileName, mPath;

public:
    void readFile();
    void readFile(QString file_name);
    void retImage(QString image_path);
    void displayImage(QImage * image);
    GameManager();
};

#endif // GAMEMANAGER_H
