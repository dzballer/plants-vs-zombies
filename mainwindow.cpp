#include "mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QTime>
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), currentPlant(NULL), currentUser(NULL), currentLevel(NULL), currentZombie(NULL), sunPoints(1000), sunTimeCounter(0), zombieCounter(0), zombieTimeCounter(0), lastZombieSpawnTime(0), zombieInterval(0), rows(5), columns(9), grid(rows, std::vector<QPointF>(columns))
{
    qsrand(QTime::currentTime().msec());

    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    gameManager = new GameManager(); // need to delete

    ui->sunpointsLabel->setText(QString::number(sunPoints));

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

    plantReady = false;

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

    // Implementing a 5x9 grid
    //vector<vector<QPointF> > grid(rows, std::vector<QPointF>(columns));
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<columns; j++)
        {
            int x = lawnLeft + j*lawnWidth/9;
            int y = lawnTop + i*lawnHeight/5;
            QPointF gridPoint;
            gridPoint.setX(x);
            gridPoint.setY(y);
            grid[i][j] = gridPoint;
        }
    }

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<columns; j++)
        {
            qDebug() << grid[i][j];
        }
    }
//gameManager->readPlayersFile(":/pvz files/pvz_players-test-2c1.csv");
//gameManager->readPlayersFile(":/pvz files/pvz_players-test-2d1.csv");
//gameManager->readPlayersFile(":/pvz files/pvz_players-test-2e1.csv");
//gameManager->readPlayersFile(":/pvz files/pvz_players-test-2e2.csv");
    gameManager->readPlayersFile(":/pvz files/pvz_players.csv"); // Reading and parsing players file
    gameManager->readLevelsFile(":/pvz files/pvz_levels.csv"); // Reading and parsing levels file
    gameManager->readPlantsFile(":/pvz files/pvz_plants.csv"); // Reading and parsing plants data
    gameManager->readZombiesFile(":/pvz files/pvz_zombies.csv"); // Reading and parsing zobies data
    users = gameManager->getUserVector();
    levels = gameManager->getLevelVector();
    plants = gameManager->getPlantVector();
    zombies = gameManager->getZombieVector();

    ui->p1Button->setIcon(QIcon(":/pvz images/" + QString::fromStdString(plants[0]->getName()) + "icon.png"));
    ui->p1Button->setIconSize(ui->p1Button->size());
    //qDebug()<<":/pvz images/" + QString::fromStdString(plants[1]->getName()) + "icon.png";
    ui->p2Button->setIcon(QIcon(":/pvz images/" + QString::fromStdString(plants[1]->getName()) + "icon.png"));
    ui->p2Button->setIconSize(ui->p2Button->size());
    ui->p3Button->setIcon(QIcon(":/pvz images/" + QString::fromStdString(plants[2]->getName()) + "icon.png"));
    ui->p3Button->setIconSize(ui->p3Button->size());
    ui->p4Button->setIcon(QIcon(":/pvz images/" + QString::fromStdString(plants[3]->getName()) + "icon.png"));
    ui->p4Button->setIconSize(ui->p4Button->size());
    ui->p5Button->setIcon(QIcon(":/pvz images/" + QString::fromStdString(plants[4]->getName()) + "icon.png"));
    ui->p5Button->setIconSize(ui->p5Button->size());
    ui->p6Button->setIcon(QIcon(":/pvz images/" + QString::fromStdString(plants[5]->getName()) + "icon.png"));
    ui->p6Button->setIconSize(ui->p6Button->size());
    ui->p7Button->setIcon(QIcon(":/pvz images/" + QString::fromStdString(plants[6]->getName()) + "icon.png"));
    ui->p7Button->setIconSize(ui->p7Button->size());
    ui->p8Button->setIcon(QIcon(":/pvz images/" + QString::fromStdString(plants[7]->getName()) + "icon.png"));
    ui->p8Button->setIconSize(ui->p8Button->size());
    //ui->p1Button->setIcon(QIcon(":/pvz images/peashootericon.png"));

    ui->userComboBox->clear();
    ui->userComboBox->addItem("Users"); // Setting initial ComboBox label to "Users" with an irrelevant index
    // *** why is it that the initial index is -1 then 0??

    for (int i = 0; i<int(users.size()); i++)
    {
        ui->userComboBox->addItem(QString::fromStdString(users[i]->getName()),i);
        //ui->userComboBox->addItem("poop",i);
    }

    //QPoint lastClick = ui->graphicsView->getPos();
}

MainWindow::~MainWindow()
{
    // save user data
    delete ui;
}

// Doesn't work because graphicsView clicks do not register in MainWindow lol
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    e->ignore(); // Using the parameter (might be useless lol)
    plantReady = false; // If user clicks anywhere outside of graphicsview, the plantReady will be terminated
}

void MainWindow::uiUpdater()
{
    if (ui->sunpointsLabel->text().toInt() != sunPoints)
    {
        ui->sunpointsLabel->setText(QString::number(sunPoints));
    }

    /*if (ui->levelLineEdit->text().toInt() != currentUser->getLevel())
    {
        ui->levelLineEdit->setText(QString::number(currentUser->getLevel()));
    }*/
}

// Runs on a timer and checks if two boolean conditions are met. Draws pixmap and adds to scene if conditions are satisfied.
void MainWindow::drawPlantChecker()
{
    if (plantReady == true && ui->graphicsView->getReady())
    {
        if (currentPlant!=NULL) // Or else comparison won't be allowed
        {
            if(sunPoints < currentPlant->getCost()) // Check if sufficient sunPoints.
            {
                qDebug() << "Not enough sunpoints.";
                plantReady = 0;
                return;
            }
        }

        QPointF lastclick = ui->graphicsView->getPos();

        int y = (lastclick.y()-lawnTop)/(lawnHeight/5); // outputting the clicked grid row x column
        int x =  (lastclick.x()-lawnLeft)/(lawnWidth/9);

        qDebug() << x << " " << y;

        // Fitting image into the click
        int gridX = lawnLeft + int(lastclick.x()-lawnLeft)/(lawnWidth/9) * (lawnWidth/9); // Allows the last click to round to the vertical line close-left to the click
        int gridY = lawnTop + int(lastclick.y()-lawnTop)/(lawnHeight/5) * (lawnHeight/5); // Allows last click to round to the horizontal line close-top to the click

        itemPos.setX(gridX);
        itemPos.setY(gridY);

        for(int i = 0; i<int(existingPlants.size()); i++)
        {
            //qDebug() << itemPos << " " << existingPlants[i] << "pop";
            if(itemPos == existingPlants[i]->pos())
            {
                qDebug() << "Error: Plant already exists";
                plantReady = 0;
                return;
            }
        }

        /*if (currentPlant->getSun() == 1)
        {
            // Set up a timer which will be responsible for dropping, deleting, and updating sunPoints
            QTimer * plantSunTimer = new QTimer(this);
            connect(plantSunTimer, SIGNAL(timeout()), this, SLOT(createSun(itemPos.x(),itemPos.y())));
            plantSunTimer->start(50);

        }*/

        //qDebug() << "Current existing plants: ";
        //for(int i=0; i<existingPlants.size(); i++)
        //    qDebug() << existingPlants[i];


        //Drawing
        QPixmap plant(":/pvz images/" + QString::fromStdString(currentPlant->getName()) + ".png");
        qDebug() << ":/pvz images/" + QString::fromStdString(currentPlant->getName()) + ".png";
        plant = plant.scaledToWidth(lawnWidth/9);
        Plant *aPlant = new Plant(currentPlant);
        aPlant->setPixmap(plant);
        scene->addItem(aPlant);
        aPlant->setPos(itemPos);

        existingPlants.push_back(aPlant);
        //QGraphicsPixmapItem * plantItem = scene->addPixmap(plant);
        //plantItem->setPixmap(QPixmap::fromImage(":/pvz images/" + QString::fromStdString(currentPlant->getName() + ".png").scaled(50,50));

        //plantItem->setPos(itemPos);
        //plantItem->setPos(ui->graphicsView->getPos());
        plantReady = false;
        //qDebug() << "draw is ready";
        sunPoints -= currentPlant->getCost();
    }

    for (int i=0; i<int(existingPlants.size()); i++)
    {
        existingPlants[i]->setShootingTimeCounter(existingPlants[i]->getShootingTimeCounter()+50);
    }

    ui->graphicsView->setReady(false); // Always turning it off so that its significance is only when clicked
}

void MainWindow::drawZombieChecker()
{
    if(zombieTimeCounter >= currentLevel->getStartTime())//*1000)
    {
        if(zombieCounter >= int(currentZombieSequence.size()))
        {
            //currentUser->setLevel(currentUser->getLevel()+1);
            zombieTimer->stop();
            return;
        }

        //zombieInterval = currentLevel->getInterval();

        if(zombieTimeCounter%1000 == 0) //zombieTimeCounter - lastZombieSpawnTime >= zombieInterval) //zombieCounter < int(currentZombieSequence.size()))
        {
            //currentZombie = new Zombie;
            currentZombie = zombies[currentZombieSequence[zombieCounter]-1]; //Using to hold current zombie
            Zombie * aZombie = new Zombie; // Need to new everytime or else same current zombie is added to scene
            //Zombie zomb = *currentZombie;
            //aZombie = *currentZombie;
            aZombie->setIndex(currentZombie->getIndex());
            aZombie->setName(currentZombie->getName());
            aZombie->setArmor(currentZombie->getArmor());
            aZombie->setLife(currentZombie->getLife());
            aZombie->setAttack(currentZombie->getAttack());
            aZombie->setAttackRate(currentZombie->getAttackRate());
            aZombie->setSpeed(currentZombie->getSpeed());

            QPixmap zombie(":/pvz images/" + QString::fromStdString(currentZombie->getName()) + ".png");

            zombie = zombie.scaledToWidth(lawnWidth/columns);
            aZombie->setPixmap(zombie);
            scene->addItem(aZombie);
            aZombie->setPos(ui->graphicsView->width()-30,lawnTop+lawnHeight/rows+50);
            qDebug() << aZombie->getSpeed();
            //aZombie->setPos(500,500);
            qDebug() << QString::fromStdString(currentZombie->getName()) << "zombie has been spawned.";
            //lastZombieSpawnTime = zombieTimeCounter + zombieInterval;
            existingZombies.push_back(aZombie);
            zombieCounter++;

            //for (int i=0; i<int(existingZombies.size()); i++)
            //{
            //    qDebug() << "zombie:"<< i << existingZombies[i]->pos();
            //}
        }
    }
    zombieTimeCounter += 50;
}

void MainWindow::sunDropper()
{
    for(int i=0; i<int(suns.size()); i++)
    {
        if(suns[i]->getDuration() >= 3000) // If sun's duration has exceeded its lifetime, it will be deleted
        {
            //vector<Sun*>::iterator iter;
            //for(int j=0; j<i; iter++, j++);
            delete suns[i];
            suns.erase(suns.begin()+i);
        }
        else if(suns[i]->getDeleteReady()) // Otherwise, if the sun is clicked, it will also be deleted
        {
            sunPoints += 25;
            delete suns[i];
            suns.erase(suns.begin()+i);
            qDebug() << "deleted";

        }

        /*if(plantSuns[i]->getDuration()>= 10000)
        {
            delete plantSuns[i];
            plantSuns.erase(plantSuns.begin()+i);
        }
        else if(plantSuns[i]->getDeleteReady())
        {
            sunPoints += 50;
            delete plantSuns[i];
            plantSuns.erase(plantSuns.begin()+i);
        }*/
    }
    /*for(int i=0; i<int(suns.size()); i++)
        {
            if(suns[i]->getSunTime() >= 200000)
            {
                //vector<Sun*>::iterator iter;
                //for(int j=0; j<i; iter++, j++);
                suns.erase(suns.begin()+i);
                delete suns[i];
            }
            else if(suns[i]->getDeleteReady())
            {
                suns.erase(suns.begin()+i);
                delete suns[i];
                qDebug() << "deleted";
            }
        }*/

    // Checking which plants are suns
    /*for(int i=0; i<int(existingPlants.size()); i++)
    {
        if (existingPlants[i]->getSun() == 1 && sunTimeCounter%5000 == 0)
        {
            Sun *sun = new Sun;
            QPixmap sunPixmap(":/pvz images/sun.png");
            sunPixmap = sunPixmap.scaledToWidth(lawnWidth/columns);
            sun->setPixmap(sunPixmap);
            scene->addItem(sun);
            sun->setPos(existingPlants[i]->pos());
            plantSuns.push_back(sun);
        }
    }*/

    if(sunTimeCounter%3000 == 0 && sunTimeCounter != 0)
    {
        Sun *sun = new Sun;
        QPixmap sunPixmap(":/pvz images/sun.png");
        sunPixmap = sunPixmap.scaledToWidth(lawnWidth/columns);
        sun->setPixmap(sunPixmap);
        scene->addItem(sun);

        // Randomizing a grid coordinate for the sundrop
        int randRow = qrand()%(rows); // /(double(RAND_MAX)+1);
        int randColumn = qrand()%(columns);

        QPointF randPoint = grid[randRow][randColumn];
        sun->setFinalPos(randPoint);
        sun->setPos(randPoint.x(), 0); // Drops from sky in a vertical fashion

        //sun->advance(10);
        //sun->setPos(itemPos);
        suns.push_back(sun);
    }
    sunTimeCounter += 50;
    for(int i=0; i<int(suns.size()); i++)
    {
        suns[i]->setDuration(suns[i]->getDuration()+50);
        //plantSuns[i]->setDuration(plantSuns[i]->getDuration()+50);
    }
}

void MainWindow::plantShooter()
{
    //plants[1]->setFireRate(2);
    for(int i=0; i<int(existingPlants.size()); i++)
    {
        if(fmod((existingPlants[i]->getShootingTimeCounter()),(1000*existingPlants[i]->getFireRate())) == 0 )
        {
            switch (existingPlants[i]->getIndex())
            {
            default:
                qDebug() << "existingPlant error";
            case 1:
            {
                Projectile *projectile = new Projectile;
                QPixmap projectilePixmap(":/pvz images/peashooterprojectile.png");
                //projectilePixmap = projectilePixmap.scaledToWidth(10);
                projectile->setPixmap(projectilePixmap);
                scene->addItem(projectile);
                projectile->setPos(existingPlants[i]->pos());

                projectiles.push_back(projectile);
                qDebug()<<QString::fromStdString(existingPlants[i]->getName()) << "shoots";
                break;
            }
            case 2:
            {
                Sun *sun = new Sun;
                QPixmap sunPixmap(":/pvz images/sun.png");
                sunPixmap = sunPixmap.scaledToWidth(lawnWidth/columns);
                sun->setPixmap(sunPixmap);
                scene->addItem(sun);
                sun->setPos(existingPlants[i]->pos());

                plantSuns.push_back(sun);
                qDebug()<<QString::fromStdString(existingPlants[i]->getName()) << "shoots";
                break;
            }
            /*case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:*/

            }
        }
    }
}

/*void MainWindow::createSun(int x, int y)
{
    Sun *aSun = new Sun;

    aSun->setPos(x,y);

    for(int i=0; i<int(suns.size()); i++)
    {
        if(suns[i]->getDuration() >= 3000)
        {
            //vector<Sun*>::iterator iter;
            //for(int j=0; j<i; iter++, j++);
            delete suns[i];
            suns.erase(suns.begin()+i);
        }
        else if(suns[i]->getDeleteReady())
        {
            sunPoints += 25;
            delete suns[i];
            suns.erase(suns.begin()+i);
            qDebug() << "deleted";
        }
    }

    if(sunTimeCounter%3000 == 0 && sunTimeCounter != 0)
    {
        Sun *sun = new Sun;
        QPixmap sunPixmap(":/pvz images/sun.png");
        sunPixmap = sunPixmap.scaledToWidth(lawnWidth/columns);
        sun->setPixmap(sunPixmap);
        scene->addItem(sun);

        // Randomizing a grid coordinate for the sundrop
        int randRow = qrand()%(rows); // /(double(RAND_MAX)+1);
        int randColumn = qrand()%(columns);

        QPointF randPoint = grid[randRow][randColumn];
        sun->setPos(randPoint);

        //sun->advance(10);
        //sun->setPos(itemPos);
        suns.push_back(sun);
    }
    sunTimeCounter += 50;
    for(int i=0; i<int(suns.size()); i++)
    {
        suns[i]->setDuration(suns[i]->getDuration()+50);
    }
}*/

void MainWindow::on_p1Button_clicked()
{
    currentPlant = plants[0]; // Sets current plant pointer to selected plant in the plants vector
    std::cout << plants[0]->getName() << " selected\n";
    plantReady = true;
}

void MainWindow::on_p2Button_clicked()
{
    currentPlant = plants[1];
    std::cout << plants[1]->getName() << " selected\n";
    plantReady = true;
}

void MainWindow::on_p3Button_clicked()
{
    currentPlant = plants[2];
    std::cout << plants[2]->getName() << " selected\n";
    plantReady = true;
}

void MainWindow::on_p4Button_clicked()
{
    currentPlant = plants[3];
    std::cout << plants[3]->getName() << " selected\n";
    plantReady = true;
}

void MainWindow::on_p5Button_clicked()
{
    currentPlant = plants[4];
    std::cout << plants[4]->getName() << " selected\n";
    plantReady = true;
}

void MainWindow::on_p6Button_clicked()
{
    currentPlant = plants[5];
    std::cout << plants[5]->getName() << " selected\n";
    plantReady = true;
}

void MainWindow::on_p7Button_clicked()
{
    currentPlant = plants[6];
    std::cout << plants[6]->getName() << " selected\n";
    plantReady = true;
}

void MainWindow::on_p8Button_clicked()
{
    currentPlant = plants[7];
    std::cout << plants[7]->getName() << " selected\n";
    plantReady = true;
}


void MainWindow::on_userComboBox_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->nameLineEdit->setText("");
        ui->levelLineEdit->setText("Select User");
        ui->deleteButton->setEnabled(0);
        //ui->startButton->setEnabled(0);
    }
    else if (index > 0) // For some reason the index is -1 and 0(?) at startup.
    {
        ui->deleteButton->setEnabled(1);
        ui->startButton->setEnabled(1);
        ui->nameLineEdit->setText(QString::fromStdString(users[index-1]->getName()));
        ui->levelLineEdit->setText("Level: " + QString::number(users[index-1]->getLevel()));
        currentUser = users[index-1];
        //currentLevel = levels[index-1];
    }
    qDebug() << index;
}

void MainWindow::on_deleteButton_clicked()
{
    if(users.size() != 0)
    {
        int currentIndex = ui->userComboBox->currentIndex();
        currentUser = users[currentIndex-1];
        delete users[currentIndex-1];
        users.erase(users.begin()+currentIndex-1);
        ui->userComboBox->removeItem(currentIndex);
    }
    else
    {
        qDebug() << "Nothing to delete.";
        return;
    }
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
    // Enabling buttons
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
    ui->startButton->setEnabled(0);
    ui->restartButton->setEnabled(1);
    ui->userComboBox->setEnabled(0);
    ui->nameLineEdit->setReadOnly(1);

    //QGraphicsScene * scene = new QGraphicsScene;

    //ui->graphicsView->setScene(scene);

    // Changing background to lawn
    QPixmap lawn(":/pvz images/lawn.jpg");
    qDebug() << lawn.width() << " " << lawn.height() << " " << lawn.width()/lawn.height();
    lawn = lawn.scaledToWidth(ui->graphicsView->width());//ui->graphicsView->width());
    scene->addPixmap(lawn);

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
    scene->addLine(QLineF() ,my_pen);
    scene->addLine(QLineF(lawnRect.bottomLeft(), lawnRect.bottomRight()) ,my_pen);
    scene->addLine(QLineF(lawnRect.topLeft(), lawnRect.topRight()) ,my_pen);
    scene->addLine(QLineF(lawnRect.bottomLeft(), lawnRect.topLeft()) ,my_pen);
    scene->addLine(QLineF(lawnRect.topRight(), lawnRect.bottomRight()) ,my_pen);

    // Drawing a grid to help with visualization
    for (int i=-1; i<9; i++)
    {
        scene->addLine(left+i*width/9,top,left+i*width/9, bottom); // drawing vertical
    }
    for (int i=0; i<6; i++)
    {
        scene->addLine(left-width/9,top+i*height/5,right,top+i*height/5); // drawing horizontal
    }

    // Set up a timer which will periodicallly call the advance() method on scene object
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawPlantChecker()));
    timer->start(50);

    // Set up a timer which will be responsible for dropping, deleting, and updating sunPoints
    sunTimer = new QTimer(this);
    connect(sunTimer, SIGNAL(timeout()), this, SLOT(sunDropper()));
    sunTimer->start(50);

    // Set up a timer which will be responsible for drawing Zombies
    zombieTimer = new QTimer(this);
    connect(zombieTimer, SIGNAL(timeout()), this, SLOT(drawZombieChecker()));
    zombieTimer->start(50); // **why is it if I leave this timer off it my function still works?

    // Set up a timer that will periodically update the UI
    uiTimer = new QTimer(this);
    connect(uiTimer, SIGNAL(timeout()), this, SLOT(uiUpdater()));
    uiTimer->start(50);

    // Set up a timer which will periodicallly call the advance() method on scene object
    sceneTimer = new QTimer(this);
    connect(sceneTimer, SIGNAL(timeout()), scene, SLOT(advance()));
    sceneTimer->start(50);

    // Set up a timer which will periodically "seed" the plants based on each plants fireRate
    shootingTimer = new QTimer(this);
    connect(shootingTimer, SIGNAL(timeout()), this, SLOT(plantShooter()));
    shootingTimer->start(50);

    // Setting currentLevel and curentZombieSequence depending on the selected user when start was clicked
    currentLevel = levels[currentUser->getLevel()-1];
    currentZombieSequence = currentLevel->getZombieSequence();

}
