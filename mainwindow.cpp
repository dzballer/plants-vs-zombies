#include "mainwindow.h"

#include <QGraphicsScene>
#include <QFileDialog>
#include <QRect>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    gameManager = new GameManager();

    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("CSV Files (*.csv);;All Files (*.*)"));
    //qDebug() << fileName;

    QGraphicsScene * scene;

    scene = new QGraphicsScene;


    // loading lawn backgorund
    //ui->graphicsView->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());
    scene->setSceneRect(0,0,ui->graphicsView->width()-2,ui->graphicsView->height()-2);;
    QPixmap lawn(":/pvz images/lawn.jpg");
    qDebug() << lawn.width() << " " << lawn.height() << " " << lawn.width()/lawn.height();
    lawn = lawn.scaledToWidth(ui->graphicsView->width());//ui->graphicsView->width());
    scene->addPixmap(lawn);
    ui->graphicsView->setScene(scene);

    // setting grid
    QPoint topLeft, bottomRight;
    topLeft.setX(0); topLeft.setY(0);
    bottomRight.setX(69); bottomRight.setY(58);
    QRect gridRect(topLeft,bottomRight);


    gameManager->readFile(":/pvz files/pvz_players.csv"); // Reading and parsing players file
    gameManager->readFile(":/pvz files/pvz_levels.csv"); // Reading and parsing levels file

}

MainWindow::~MainWindow()
{
    // save user data
    delete ui;
}

void MainWindow::on_userButton_clicked()
{

}

void MainWindow::on_p1Button_clicked()
{

}

void MainWindow::on_p2Button_clicked()
{

}

void MainWindow::on_p3Button_clicked()
{

}

void MainWindow::on_p4Button_clicked()
{

}

void MainWindow::on_p5Button_clicked()
{

}

void MainWindow::on_p6Button_clicked()
{

}

void MainWindow::on_p7Button_clicked()
{

}

void MainWindow::on_p8Button_clicked()
{

}
