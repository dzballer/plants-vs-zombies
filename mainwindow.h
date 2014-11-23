#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamemanager.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "lawn.h"
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_userButton_clicked();

    void on_p1Button_clicked();

    void on_p3Button_clicked();

    void on_p2Button_clicked();

    void on_p4Button_clicked();

    void on_p5Button_clicked();

    void on_p6Button_clicked();

    void on_p7Button_clicked();

    void on_p8Button_clicked();
private:
    Ui::MainWindow *ui;
    GameManager * gameManager;
    int plantIndex;
};

#endif // MAINWINDOW_H
