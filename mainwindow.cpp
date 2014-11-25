#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    gameManager = new GameManager(); // need to delete

    ui->levelLineEdit->setReadOnly(1);
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("CSV Files (*.csv);;All Files (*.*)"));
    //qDebug() << fileName;

    QGraphicsScene * scene;

    scene = new QGraphicsScene; // need to delete

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
    std::cout << plants[0]->getName() << " selected\n";
}

void MainWindow::on_p2Button_clicked()
{
    currentPlant = plants[1];
    std::cout << plants[1]->getName() << " selected\n";
}

void MainWindow::on_p3Button_clicked()
{
    currentPlant = plants[2];
    std::cout << plants[2]->getName() << " selected\n";
}

void MainWindow::on_p4Button_clicked()
{
    currentPlant = plants[3];
    std::cout << plants[3]->getName() << " selected\n";
}

void MainWindow::on_p5Button_clicked()
{
    currentPlant = plants[4];
    std::cout << plants[4]->getName() << " selected\n";
}

void MainWindow::on_p6Button_clicked()
{
    currentPlant = plants[5];
    std::cout << plants[5]->getName() << " selected\n";
}

void MainWindow::on_p7Button_clicked()
{
    currentPlant = plants[6];
    std::cout << plants[6]->getName() << " selected\n";
}

void MainWindow::on_p8Button_clicked()
{
    currentPlant = plants[7];
    std::cout << plants[7]->getName() << " selected\n";
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
    users.push_back(newUser);

    ui->userComboBox->addItem(QString::fromStdString(newUser->getName()), int(users.size())); // Adding user to the userComboBox
}
