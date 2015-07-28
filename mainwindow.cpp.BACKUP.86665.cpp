#include "mainwindow.h"
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    gameManager = new GameManager(); // need to delete

    ui->p1Button->setEnabled(0);
    ui->p2Button->setEnabled(0);
    ui->p3Button->setEnabled(0);
    ui->p4Button->setEnabled(0);
    ui->p5Button->setEnabled(0);
    ui->p6Button->setEnabled(0);
    ui->p7Button->setEnabled(0);
    ui->p8Button->setEnabled(0);
    ui->restartButton->setEnabled(0);
    ui->levelLineEdit->setReadOnly(1);

    QGraphicsScene * scene;

    scene = new QGraphicsScene; // need to delete
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setStyleSheet("background: black");
    // loading lawn backgorund
    //ui->graphicsView->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());
    scene->setSceneRect(0,0,ui->graphicsView->width()-2,ui->graphicsView->height()-2);;
    QPixmap titlePage(":/pvz images/titlepage.png");
    titlePage = titlePage.scaledToHeight(ui->graphicsView->height());//ui->graphicsView->width());
    QGraphicsPixmapItem *pm = scene->addPixmap(titlePage);
    //qDebug() << titlePage.width() << " " << titlePage.height();
    pm->setPos((ui->graphicsView->width()-titlePage.width())/2, 0);




    // buttons disabled before user and level are set
   // ui->Button->setEnabled(false); // disables the forward and backward buttons because they have no function before file is loaded
    //ui->backwardButton->setEnabled(false);

//gameManager->readPlayersFile(":/pvz files/pvz_players-test-2c1.csv");
//gameManager->readPlayersFile(":/pvz files/pvz_players-test-2d1.csv");
//gameManager->readPlayersFile(":/pvz files/pvz_players-test-2e1.csv");
//gameManager->readPlayersFile(":/pvz files/pvz_players-test-2e2.csv");
    gameManager->readPlayersFile(":/pvz files/pvz_players.csv"); // Reading and parsing players file
    gameManager->readLevelsFile(":/pvz files/pvz_levels.csv"); // Reading and parsing levels file
    gameManager->readPlantsFile(":/pvz files/pvz_plants.csv"); // Reading and parsing plants data

    users = gameManager->getUserVector();
    levels = gameManager->getLevelVector();
    plants = gameManager->getPlantVector();

    ui->userComboBox->clear();
    ui->userComboBox->addItem("Users"); // Setting initial ComboBox label to "Users" with an irrelevant index
    // *** why is it that the initial index is -1 then 0??

    for (int i = 0; i<int(users.size()); i++)
    {
        ui->userComboBox->addItem(QString::fromStdString(users[i]->getName()),i);
        //ui->userComboBox->addItem("poop",i);
    }
}

MainWindow::~MainWindow()
{
    // save user data
    delete ui;
}

void MainWindow::on_p1Button_clicked()
{
    currentPlant = plants[0]; // Sets current plant pointer to selected plant in the plants vector
<<<<<<< HEAD
    std::cout << plants[0]->getName() << " selected\n";
=======
    std::cout << plants[0]->getName() << "selected\n";
>>>>>>> b2d13ec0234c8c403753a66bbe156491f17cf75c
}

void MainWindow::on_p2Button_clicked()
{
<<<<<<< HEAD
    currentPlant = plants[1];
    std::cout << plants[1]->getName() << " selected\n";
=======
    currentPlant = plants[0];
    std::cout << plants[0]->getName() << "selected\n";
>>>>>>> b2d13ec0234c8c403753a66bbe156491f17cf75c
}

void MainWindow::on_p3Button_clicked()
{
<<<<<<< HEAD
    currentPlant = plants[2];
    std::cout << plants[2]->getName() << " selected\n";
=======
    currentPlant = plants[0];
    std::cout << plants[0]->getName() << "selected\n";
>>>>>>> b2d13ec0234c8c403753a66bbe156491f17cf75c
}

void MainWindow::on_p4Button_clicked()
{
<<<<<<< HEAD
    currentPlant = plants[3];
    std::cout << plants[3]->getName() << " selected\n";
=======
    currentPlant = plants[0];
    std::cout << plants[0]->getName() << "selected\n";
>>>>>>> b2d13ec0234c8c403753a66bbe156491f17cf75c
}

void MainWindow::on_p5Button_clicked()
{
<<<<<<< HEAD
    currentPlant = plants[4];
    std::cout << plants[4]->getName() << " selected\n";
=======
    currentPlant = plants[0];
    std::cout << plants[0]->getName() << "selected\n";
>>>>>>> b2d13ec0234c8c403753a66bbe156491f17cf75c
}

void MainWindow::on_p6Button_clicked()
{
<<<<<<< HEAD
    currentPlant = plants[5];
    std::cout << plants[5]->getName() << " selected\n";
=======
    currentPlant = plants[0];
    std::cout << plants[0]->getName() << "selected\n";
>>>>>>> b2d13ec0234c8c403753a66bbe156491f17cf75c
}

void MainWindow::on_p7Button_clicked()
{
<<<<<<< HEAD
    currentPlant = plants[6];
    std::cout << plants[6]->getName() << " selected\n";
=======
    currentPlant = plants[0];
    std::cout << plants[0]->getName() << "selected\n";
>>>>>>> b2d13ec0234c8c403753a66bbe156491f17cf75c
}

void MainWindow::on_p8Button_clicked()
{
<<<<<<< HEAD
    currentPlant = plants[7];
    std::cout << plants[7]->getName() << " selected\n";
=======
    currentPlant = plants[0];
    std::cout << plants[0]->getName() << "selected\n";
>>>>>>> b2d13ec0234c8c403753a66bbe156491f17cf75c
}


void MainWindow::on_userComboBox_currentIndexChanged(int index)
{
    if (index > 0) // For some reason the index is -1 and 0(?) at startup.
    {
        ui->nameLineEdit->setText(QString::fromStdString(users[index-1]->getName()));
        ui->levelLineEdit->setText("Level: " + QString::number(users[index-1]->getLevel()));
    }
    qDebug() << index;
}

void MainWindow::on_deleteButton_clicked()
{
    int currentIndex = ui->userComboBox->currentIndex();
    currentUser = users[currentIndex-1];
    delete users[currentIndex-1];
    users.erase(users.begin()+currentIndex-1);
    ui->userComboBox->removeItem(currentIndex);
}

void MainWindow::on_newButton_clicked()
{
    User * newUser = new User;
    newUser->setName(ui->nameLineEdit->text().toStdString()); // Setting name of user to the typed name on nameLineEdit
    newUser->setLevel(1); // Setting level to 1 or else newUser gets random level.
    users.push_back(newUser);

    ui->userComboBox->addItem(QString::fromStdString(newUser->getName()), int(users.size())); // Adding user to the userComboBox
}

void MainWindow::on_startButton_clicked()
{
    ui->p1Button->setEnabled(1);
    ui->p2Button->setEnabled(1);
    ui->p3Button->setEnabled(1);
    ui->p4Button->setEnabled(1);
    ui->p5Button->setEnabled(1);
    ui->p6Button->setEnabled(1);
    ui->p7Button->setEnabled(1);
    ui->p8Button->setEnabled(1);
    ui->newButton->setEnabled(0);
    ui->deleteButton->setEnabled(0);
    ui->restartButton->setEnabled(1);
    ui->userComboBox->setEnabled(0);
    ui->nameLineEdit->setReadOnly(1);

    QGraphicsScene * startScene = new QGraphicsScene;
    ui->graphicsView->setScene(startScene);

    // Changing background to lawn
    QPixmap lawn(":/pvz images/lawn.jpg");
    qDebug() << lawn.width() << " " << lawn.height() << " " << lawn.width()/lawn.height();
    lawn = lawn.scaledToWidth(ui->graphicsView->width());//ui->graphicsView->width());
    startScene->addPixmap(lawn);

    // setting grid
    QPoint topLeft, bottomRight;
    topLeft.setX(0); topLeft.setY(0);
    bottomRight.setX(69); bottomRight.setY(58);
    QRectF gridRect(topLeft,bottomRight);

    // setting lawn parameters
    int left = 180, right = 700, top = 55, bottom = 410;
    int width = right-left, height = bottom-top;
    QRectF lawnRect(left,top,width,height); // Creating a lawn rectangle with bounds left,up,width,height

    // Draw lines to help out with general visualization
    QPen my_pen = QPen(Qt::black);
    startScene->addLine(QLineF() ,my_pen);
    startScene->addLine(QLineF(lawnRect.bottomLeft(), lawnRect.bottomRight()) ,my_pen);
    startScene->addLine(QLineF(lawnRect.topLeft(), lawnRect.topRight()) ,my_pen);
    startScene->addLine(QLineF(lawnRect.bottomLeft(), lawnRect.topLeft()) ,my_pen);
    startScene->addLine(QLineF(lawnRect.topRight(), lawnRect.bottomRight()) ,my_pen);

    // Drawing a grid to help with visualization
    for (int i=-1; i<9; i++)
    {
        startScene->addLine(left+i*width/9,top,left+i*width/9, bottom); // drawing vertical
    }
    for (int i=0; i<6; i++)
    {
        startScene->addLine(left-width/9,top+i*height/5,right,top+i*height/5); // drawing horizontal
    }



}
