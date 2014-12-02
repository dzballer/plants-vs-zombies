#include "mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QTime>
#include <math.h>
#include <QMessageBox>
#include <QTextItem>
#include <QApplication>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), currentPlant(NULL), currentUser(NULL), currentLevel(NULL),
    currentZombie(NULL), plantReady(false), sunPoints(1000), sunTimeCounter(0), zombieCounter(0),
    zombieTimeCounter(0), lastZombieSpawnTime(0), zombieInterval(0), rows(5), columns(9), grid(rows, std::vector<QPointF>(columns)), levelEnd(false)
{
    qsrand(QTime::currentTime().msec());

    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));

    // Creating gameManager that loads/reads files and data
    gameManager = new GameManager(); // need to delete

    // Setting progressbar ranges for seeding times
    ui->progressBar_1->setRange(0,7500);
    ui->progressBar_2->setRange(0,7500);
    ui->progressBar_3->setRange(0,50000);
    ui->progressBar_4->setRange(0,30000);
    ui->progressBar_5->setRange(0,30000);
    ui->progressBar_6->setRange(0,7500);
    ui->progressBar_7->setRange(0,7500);
    ui->progressBar_8->setRange(0,7500);

    // Setting buttons to checkable
    setButtonsCheckable(1);

    // Setting ui labels and button usability
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

    //Loading csv files from resource file
    //Testplan loading other csv files
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

    // Setting planticons for the plant buttons
    ui->p1Button->setIcon(QIcon(":/pvz images/" + QString::fromStdString(plants[0]->getName()) + "icon.png"));
    ui->p1Button->setIconSize(ui->p1Button->size());
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

    // Setting scene to a titlepage
    scene = new QGraphicsScene; // need to delete
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setStyleSheet("background: black");
    scene->setSceneRect(0,0,ui->graphicsView->width()-2,ui->graphicsView->height()-2);;
    QPixmap titlePage(":/pvz images/titlepage.png");
    titlePage = titlePage.scaledToHeight(ui->graphicsView->height());
    QGraphicsPixmapItem *pm = scene->addPixmap(titlePage);
    pm->setPos((ui->graphicsView->width()-titlePage.width())/2, 0);

    // Sorting users by timestamp using bubblesort
    sortComboBox();

    // Showing that users are indeed sorted properly
    for (int i=0; i<int(users.size());i++)
    {
        qDebug() << users.at(i)->getTimeStamp();
    }

    // Adding users
    ui->userComboBox->clear();
    ui->userComboBox->addItem("Select/Create User"); // Setting initial ComboBox label to "Users" with an irrelevant index
    // *** why is it that the initial index is -1 then 0??

    // Adding all user names to the combobox in already sorted order from most recent to least recent
    for (int i = 0; i<int(users.size()); i++)
    {
        ui->userComboBox->addItem(QString::fromStdString(users.at(i)->getName()),i);
    }
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
    setButtonsCheck(0);
}

void MainWindow::sortComboBox()
{
    User *temp;
    for (int i=0; i<int(users.size()); i++)
    {
        for (int j=0; j<int(users.size()-1); j++)
        {
            if(users.at(j)->getTimeStamp() < users[j+1]->getTimeStamp())
             {
                temp = users.at(j);
                users.at(j) = users[j+1];
                users[j+1] = temp;
             }
        }
    }
    temp = NULL;
}

void MainWindow::setButtonsCheckable(bool checkable)
{
    ui->p1Button->setCheckable(checkable);
    ui->p2Button->setCheckable(checkable);
    ui->p3Button->setCheckable(checkable);
    ui->p4Button->setCheckable(checkable);
    ui->p5Button->setCheckable(checkable);
    ui->p6Button->setCheckable(checkable);
    ui->p7Button->setCheckable(checkable);
    ui->p8Button->setCheckable(checkable);
}

void MainWindow::setButtonsCheck(bool check)
{
    ui->p1Button->setChecked(check);
    ui->p2Button->setChecked(check);
    ui->p3Button->setChecked(check);
    ui->p4Button->setChecked(check);
    ui->p5Button->setChecked(check);
    ui->p6Button->setChecked(check);
    ui->p7Button->setChecked(check);
    ui->p8Button->setChecked(check);
}

void MainWindow::uiUpdater()
{
    // Updating sunpoints
    if (ui->sunpointsLabel->text().toInt() != sunPoints)
    {
        ui->sunpointsLabel->setText(QString::number(sunPoints));
    }

    // Updating progress bar values
    if (ui->progressBar_1->value() <= ui->progressBar_1->maximum())
    {
        ui->progressBar_1->setValue(ui->progressBar_1->value()+50);
    }
    if (ui->progressBar_2->value() <= ui->progressBar_2->maximum())
    {
        ui->progressBar_2->setValue(ui->progressBar_2->value()+50);
    }
    if (ui->progressBar_3->value() <= ui->progressBar_3->maximum())
    {
        ui->progressBar_3->setValue(ui->progressBar_3->value()+50);
    }
    if (ui->progressBar_4->value() <= ui->progressBar_4->maximum())
    {
        ui->progressBar_4->setValue(ui->progressBar_4->value()+50);
    }
    if (ui->progressBar_5->value() <= ui->progressBar_5->maximum())
    {
        ui->progressBar_5->setValue(ui->progressBar_5->value()+50);
    }
    if (ui->progressBar_6->value() <= ui->progressBar_6->maximum())
    {
        ui->progressBar_6->setValue(ui->progressBar_6->value()+50);
    }
    if (ui->progressBar_7->value() <= ui->progressBar_7->maximum())
    {
        ui->progressBar_7->setValue(ui->progressBar_7->value()+50);
    }
    if (ui->progressBar_8->value() <= ui->progressBar_8->maximum())
    {
        ui->progressBar_8->setValue(ui->progressBar_8->value()+50);
    }

    // *** progress bars check
    // Updating buttons depending on sufficient sunpoints and progress bar status
    /*if (sunPoints < plants[0]->getCost() || ui->progressBar_1->value() < .99*ui->progressBar_1->maximum()) // Progress bar only goes up to ~99% of maximum
    {
        //ui->p1Button->setEnabled(0);
    }
    else
    {
        ui->p1Button->setEnabled(1);
    }
    if (sunPoints < plants[1]->getCost() || ui->progressBar_2->value() < .99*ui->progressBar_2->maximum() )
    {
        ui->p2Button->setEnabled(0);
    }
    else
    {
        ui->p2Button->setEnabled(1);
    }
    if (sunPoints < plants[2]->getCost() || ui->progressBar_3->value() < .99*ui->progressBar_3->maximum())
    {
        ui->p3Button->setEnabled(0);
    }
    else
    {
        ui->p3Button->setEnabled(1);
    }
    if (sunPoints < plants[3]->getCost() || ui->progressBar_4->value() < .99*ui->progressBar_4->maximum())
    {
        ui->p4Button->setEnabled(0);
    }
    else
    {
        ui->p4Button->setEnabled(1);
    }
    if (sunPoints < plants[4]->getCost() || ui->progressBar_5->value() < .99*ui->progressBar_5->maximum())
    {
        ui->p5Button->setEnabled(0);
    }
    else
    {
        ui->p5Button->setEnabled(1);
    }
    if (sunPoints < plants[5]->getCost() || ui->progressBar_6->value() < .99*ui->progressBar_6->maximum())
    {
        ui->p6Button->setEnabled(0);
    }
    else
    {
        ui->p6Button->setEnabled(1);
    }
    if (sunPoints < plants[6]->getCost() || ui->progressBar_7->value() < .99*ui->progressBar_7->maximum())
    {
        ui->p7Button->setEnabled(0);
    }
    else
    {
        ui->p7Button->setEnabled(1);
    }
    if (sunPoints < plants[7]->getCost() || ui->progressBar_8->value() < .99*ui->progressBar_8->maximum())
    {
        ui->p8Button->setEnabled(0);
    }
    else
    {
        ui->p8Button->setEnabled(1);
    }*/


    /*if (ui->levelLineEdit->text().toInt() <= currentUser->getLevel())
    {
        ui->levelLineEdit->setText(QString::number(currentUser->getLevel()));
    }*/
}

// Runs on a timer and checks if two boolean conditions are met. Draws pixmap and adds to scene if conditions are satisfied.
void MainWindow::drawPlantChecker()
{
    // If plant has been selected, below statement will turn true when user clicks the lawn
    if (plantReady == true && ui->graphicsView->getReady())
    {
        qDebug() << "about to plant";
        if (currentPlant==NULL)
        {
            return;
        }

        if(sunPoints < currentPlant->getCost()) // Check if sufficient sunPoints.
        {
            qDebug() << "Not enough sunpoints.";
            plantReady = 0;
            setButtonsCheck(0);
            return;
        }

        QPointF lastclick = ui->graphicsView->getPos(); // Getting position of click, not the actual graphics view - see lawn.cpp

        //int y = (lastclick.y()-lawnTop)/(lawnHeight/5); // outputting the clicked grid row x column
        //int x =  (lastclick.x()-lawnLeft)/(lawnWidth/9);

        // Checking if click is within x bounds of lawn
        if (lastclick.x() < lawnLeft || lastclick.x() > lawnRight)
        {
            qDebug() << "Must plant on grass";
            plantReady = 0;
            setButtonsCheck(0);
            return;
        }

        // Fitting image into the grid (by setting/rounding coordinate to top left)
        int gridX = lawnLeft + int((lastclick.x()-lawnLeft)/(lawnWidth/9)) * (lawnWidth/9); // Allows the last click to round to the vertical line close-left to the click, -10 used for some kind of rounding error
        int gridY = lawnTop + int((lastclick.y()-lawnTop)/(lawnHeight/5)) * (lawnHeight/5); // Allows last click to round to the horizontal line close-top to the click
        itemPos.setX(gridX);
        itemPos.setY(gridY);

        qDebug() << itemPos.x();

        // Checking y bounds of lawn (which depends on level)
        if(currentLevel->getLevelNumber() == 1) // If level 1
        {
            for(int i=0; i<9; i++)
            {
                if(itemPos.y() != grid.at(2).at(i).y()) // Checking middle row
                {
                    qDebug() << "Must plant on grass";
                    plantReady = 0;
                    setButtonsCheck(0);
                    return;
                }
            }
        }
        else if(currentLevel->getLevelNumber() == 2) // If level 2
        {
            for(int i = 0; i<3; i++) // Checking 3 rows
            {
                for(int j = 0; j<9; j++)
                {
                    if(itemPos.y() != grid[1].at(j).y() && itemPos.y() != grid[2].at(j).y() && itemPos.y() != grid[3].at(j).y())  // Checking rows 2-4
                    {
                        qDebug() << "Must plant on grass.";
                        plantReady = 0;
                        setButtonsCheck(0);
                        return;
                    }
                }
            }
        }
        else if(currentLevel->getLevelNumber() == 3) // If level 3
        {
            for(int i = 0; i<5; i++) // Checking 5 rows
            {
                for(int j = 0; j<9; j++)
                {
                    if(itemPos.y() > grid[0].at(j).y() || itemPos.y() < grid[4].at(j).y())  // Checking rows 2-4
                    {
                        qDebug() << "Must plant on grass.";
                        plantReady = 0;
                        setButtonsCheck(0);
                        return;
                    }
                }
            }
        }

        // If plant is repeater and chosen square is empty
        if(currentPlant->getIndex() == 8)
        {
            bool isEmpty = true;
            for(int i=0; i<int(existingPlants.size()); i++)
            {
                if(itemPos == existingPlants.at(i)->pos())
                {
                    isEmpty = false;
                    break;
                }
            }
            if(isEmpty)
            {
                qDebug() << "Can only be upgraded from a peashooter";
                plantReady = 0;
                setButtonsCheck(0);
                return;
            }
        }

        // Checking if plant already exists in spot and checks for repeater
        // 2 checks in 1 loop - had to do this or else program would plant on empty
        for(int i = 0; i<int(existingPlants.size()); i++)
        {   
            // Checking for repeater

            if(currentPlant->getIndex() == 8)
            {
                if(itemPos == existingPlants.at(i)->pos()) // If chosen square has plant
                {
                    if(existingPlants.at(i)->getIndex() == 1) // if chosen existing plant is peashooter, pass check
                    {
                        // we will "remove" the peashooter, and pass the check so that repeater will be planted
                        existingPlants.at(i)->hide();
                        existingPlants.at(i)->setAlive(false);
                        existingPlants.at(i)->setPos(2000,existingPlants.at(i)->pos().y()); // Removing it from view
                        existingPlants.at(i)->getLifeLabel()->setPos(2000,existingPlants.at(i)->pos().y()); // Removing it from view
                        break;
                    }
                    else if(existingPlants.at(i)->getIndex() !=1) // If the chosen square's plant is not peashooter
                    {
                        qDebug() << "Cannot be upgraded from" << QString::fromStdString(existingPlants.at(i)->getName());
                        plantReady = 0;
                        setButtonsCheck(0);
                        return;
                    }
                }
            }
            else if(currentPlant->getIndex() != 8) // if not repeater, then will just check for existing plant
            {
                if(itemPos == existingPlants.at(i)->pos()) // if existing plant
                {
                    qDebug() << "Must plant on empty square.";
                    plantReady = 0;
                    setButtonsCheck(0);
                    return;
                }
            }
        }

        // If all checks are passed, function will draw plant
        QPixmap plant(":/pvz images/" + QString::fromStdString(currentPlant->getName()) + ".png");
        qDebug() << ":/pvz images/" + QString::fromStdString(currentPlant->getName()) + ".png";
        plant = plant.scaledToWidth(lawnWidth/9);
        Plant *aPlant = new Plant(currentPlant);
        aPlant->setPixmap(plant);
        scene->addItem(aPlant);
        aPlant->setPos(itemPos);

        QGraphicsTextItem * lifeLabel = aPlant->getLifeLabel();
        lifeLabel->setPlainText("HP:" + QString::number(aPlant->getLife()));
        scene->addItem(lifeLabel);
        lifeLabel->setPos(aPlant->pos().x()+20,aPlant->pos().y()-10);
        lifeLabel->setDefaultTextColor(Qt::red);
        existingPlants.push_back(aPlant);

        //qDebug() << aPlant->getAlive();

        plantReady = false;
        setButtonsCheck(0);

        // Taking away sunpoints
        sunPoints -= currentPlant->getCost();

        // Resets progress bar timer when current plant is planted
        if(currentPlant->getIndex() == 1)
        {
            //ui->progressBar_1->setValue(0);
        }
        else if(currentPlant->getIndex() == 2)
        {
            ui->progressBar_2->setValue(0);
        }
        else if(currentPlant->getIndex() == 3)
        {
            ui->progressBar_3->setValue(0);
        }
        else if(currentPlant->getIndex() == 4)
        {
            ui->progressBar_4->setValue(0);
        }
        else if(currentPlant->getIndex() == 5)
        {
            ui->progressBar_5->setValue(0);
        }
        else if(currentPlant->getIndex() == 6)
        {
            ui->progressBar_6->setValue(0);
        }
        else if(currentPlant->getIndex() == 7)
        {
            ui->progressBar_7->setValue(0);
        }
        else if(currentPlant->getIndex() == 8)
        {
            ui->progressBar_8->setValue(0);
        }
    }

    for (int i=0; i<int(existingPlants.size()) && !existingPlants.empty(); i++)
    {
        existingPlants.at(i)->setShootingTimeCounter(existingPlants.at(i)->getShootingTimeCounter()+50);
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

        if(zombieTimeCounter%500 == 0) //zombieTimeCounter - lastZombieSpawnTime >= zombieInterval) //zombieCounter < int(currentZombieSequence.size()))
        {
            if (currentLevel->getLevelNumber() == 1)
            {
                for (int i=0; i<currentLevel->getRows(); i++) // 1 iterations
                {
                    // Creating Zombies
                    currentZombie = zombies[currentZombieSequence[zombieCounter]-1]; //Using to hold current zombie
                    Zombie * aZombie = new Zombie(currentZombie); // Need to new everytime or else same current zombie is added to scene

                    QPixmap zombie(":/pvz images/" + QString::fromStdString(currentZombie->getName()) + ".png");

                    zombie = zombie.scaledToHeight(lawnHeight/5); // More appropriate than width based on zombie height variation
                    aZombie->setPixmap(zombie);
                    scene->addItem(aZombie);
                    aZombie->setPos(ui->graphicsView->width(),lawnTop+2*lawnHeight/(5)); //Middle row

                    //QGraphicsTextItem * lifeLabel = aZombie->getLifeLabel();
                    //lifeLabel->setPlainText("HP:" + QString::number(aZombie->getLife()));
                    //scene->addItem(lifeLabel);
                    //lifeLabel->setPos(aZombie->pos().x()+20,aZombie->pos().y()-10);
                    //lifeLabel->setDefaultTextColor(Qt::red);

                    //qDebug() << aZombie->pos();
                    qDebug() << QString::fromStdString(currentZombie->getName()) << "zombie has been spawned.";
                    existingZombies.push_back(aZombie);
                }
                zombieCounter++;
            }
            else if(currentLevel->getLevelNumber() == 2)
            {
                //for (int i=0; i<currentLevel->getRows(); i++) // 3 iterations
                //{
                    // Creating Zombies
                    currentZombie = zombies[currentZombieSequence[zombieCounter]-1]; //Using to hold current zombie
                    Zombie * aZombie = new Zombie(currentZombie); // Need to new everytime or else same current zombie is added to scene

                    QPixmap zombie(":/pvz images/" + QString::fromStdString(currentZombie->getName()) + ".png");

                    zombie = zombie.scaledToHeight(lawnHeight/5);
                    aZombie->setPixmap(zombie);
                    scene->addItem(aZombie);
                    aZombie->setPos(ui->graphicsView->width(),lawnTop+(qrand()%3+1)*lawnHeight/5); // Zombie spawned in random row

                    qDebug() << QString::fromStdString(currentZombie->getName()) << "zombie has been spawned.";
                    existingZombies.push_back(aZombie);


                    /*QGraphicsTextItem * lifeLabel = aZombie->getLifeLabel();
                    lifeLabel->setPlainText("HP:" + QString::number(aZombie->getLife()));
                    scene->addItem(lifeLabel);
                    lifeLabel->setPos(aZombie->pos().x()+20,aZombie->pos().y()-10);
                    lifeLabel->setDefaultTextColor(Qt::red);*/
                //}
                zombieCounter++;
            }
            else if(currentLevel->getLevelNumber() == 3)
            {
                //for (int i=0; i<5; i++) // 5 iterations
                //{
                    // Creating Zombies
                    currentZombie = zombies[currentZombieSequence[zombieCounter]-1]; //Using to hold current zombie
                    Zombie * aZombie = new Zombie(currentZombie); // Need to new everytime or else same current zombie is added to scene

                    QPixmap zombie(":/pvz images/" + QString::fromStdString(currentZombie->getName()) + ".png");

                    zombie = zombie.scaledToHeight(lawnHeight/5);
                    aZombie->setPixmap(zombie);
                    scene->addItem(aZombie);
                    aZombie->setPos(ui->graphicsView->width()-30,lawnTop+(qrand()%5)*lawnHeight/5); // Random row
                    qDebug() << aZombie->getSpeed();
                    qDebug() << QString::fromStdString(currentZombie->getName()) << "zombie has been spawned.";
                    existingZombies.push_back(aZombie);
                    /*QGraphicsTextItem * lifeLabel = aZombie->getLifeLabel();
                    lifeLabel->setPlainText("HP:" + QString::number(aZombie->getLife()));
                    scene->addItem(lifeLabel);
                    lifeLabel->setPos(aZombie->pos().x()+20,aZombie->pos().y()-10);
                    lifeLabel->setDefaultTextColor(Qt::red);*/
                //}
                zombieCounter++;
            }
        }
    }
    zombieTimeCounter += 50;
}

void MainWindow::sunDropper()
{
    for(int i=0; i<int(suns.size()); i++)
    {
        if(suns.at(i)->getDuration() >= 7500) // If sun's duration has exceeded 7.5s, it will be deleted
        {
            delete suns.at(i);
            suns.erase(suns.begin()+i);
        }
        else if(suns.at(i)->getDeleteReady()) // Otherwise, if the sun is clicked, it will also be deleted
        {
            sunPoints += 25;
            delete suns.at(i);
            suns.erase(suns.begin()+i);
            qDebug() << "sun deleted";
        }
    }

    if(sunTimeCounter%10000 == 0 && sunTimeCounter != 0) // Every 10 seconds but not at very start
    {
        // Creating sun
        Sun *sun = new Sun;
        QPixmap sunPixmap(":/pvz images/sun.png");
        sunPixmap = sunPixmap.scaledToWidth(lawnWidth/columns);
        sun->setPixmap(sunPixmap);
        scene->addItem(sun);

        // Randomizing a grid coordinate for the sundrop
        int randRow, randColumn;

        // Available rows will depend on the level
        if(currentLevel->getLevelNumber() == 1)
        {
            randRow = 2; // Middle row
            randColumn = qrand()%(columns);
        }
        else if(currentLevel->getLevelNumber() == 2)
        {
            randRow = 1+qrand()%3; // Rows 2-4
            randColumn = qrand()%(columns);
        }
        else if(currentLevel->getLevelNumber() == 3)
        {
            randRow = qrand()%5; // Rows 1-5
            randColumn = qrand()%(columns);
        }
        else
        {
            // for debugging
            randRow = 0;
            randColumn = 0;
        }

        QPointF randPoint = grid[randRow][randColumn];

        sun->setFinalPos(randPoint);
        sun->setPos(randPoint.x(), 0); // Drops from sky in a vertical fashion
        suns.push_back(sun); // Keeps track of existing suns
    }
    sunTimeCounter += 50;
    for(int i=0; i<int(suns.size()); i++)
    {
        if (suns.at(i)->getFinalPos() == suns.at(i)->pos()) // Once sun is at rest
        {
            suns.at(i)->setDuration(suns.at(i)->getDuration()+50); // Adds to each sun's timeCounter/duration
        }
    }
}

void MainWindow::plantShooter()
{
    //plants[1]->setFireRate(2);
    for(int i=0; i<int(existingPlants.size()) && !existingPlants.empty(); i++)
    {
        //qDebug() <<"in for loop";
        //qDebug() << "Plant:" <<i << existingPlants.at(i)->getAlive();
        //qDebug() << existingPlants.at(i)->getShootingTimeCounter();

        if(existingPlants.at(i)->getAlive() && fmod((existingPlants.at(i)->getShootingTimeCounter()),(1000*existingPlants.at(i)->getFireRate())) == 0 )
        {
            qDebug() << existingPlants.at(i)->getLife();
            switch (existingPlants.at(i)->getIndex())
            {
            default:
                qDebug() << "existingPlant error";

            case 1:
            {
                qDebug() << "new projectile";
                Projectile *projectile = new Projectile;
                QPixmap projectilePixmap(":/pvz images/peashooterprojectile.png");
                //projectilePixmap = projectilePixmap.scaledToWidth(10);
                projectile->setPixmap(projectilePixmap);
                scene->addItem(projectile);
                projectile->setPos(existingPlants.at(i)->pos().x()+40, existingPlants.at(i)->pos().y());

                projectiles.push_back(projectile);
                qDebug()<<QString::fromStdString(existingPlants.at(i)->getName()) << "shoots";
                break;
            }
            case 2:
            {   
                Sun *sun = new Sun;
                QPixmap sunPixmap(":/pvz images/sun.png");
                sunPixmap = sunPixmap.scaledToWidth(lawnWidth/columns);
                sun->setPixmap(sunPixmap);
                scene->addItem(sun);
                sun->setPos(existingPlants.at(i)->pos());

                plantSuns.push_back(sun);
                qDebug()<<QString::fromStdString(existingPlants.at(i)->getName()) << "shoots";
                break;
            }
            case 3:
            {
                QPointF plantPos = existingPlants.at(i)->pos();
                int leftBound = plantPos.x() - lawnWidth/9;
                int rightBound = plantPos.x() + 2*lawnWidth/9; // covers all zombies in 3x3 since zombie pos is topleft
                int topBound = plantPos.y() - lawnHeight/5;
                int bottomBound = plantPos.y() + lawnHeight/5;
                for (int j=0; j<int(existingZombies.size()); j++)
                {
                    QPointF zombiePos = existingZombies.at(j)->pos();
                    if(zombiePos.x() >= leftBound && zombiePos.x() <= rightBound && zombiePos.y() >= topBound && zombiePos.y() <= bottomBound)
                    {
                        existingZombies.at(j)->hide();
                        existingZombies.at(j)->setAlive(false);
                    }
                }
                existingPlants.at(i)->hide();
                existingPlants.at(i)->setAlive(false);
                existingPlants.at(i)->setPos(2000,existingPlants.at(i)->pos().y()); // Removing it from view
                existingPlants.at(i)->getLifeLabel()->setPos(2000,existingPlants.at(i)->pos().y()); // Removing it from view
                break;
            }
            case 4:
            {
                break;
            }
            case 5: // need to fix
            {
                for (int j=0; j<int(existingZombies.size()); j++)
                {
                    //QPointF zombiePos = existingZombies.at(j)->pos();
                    //if(zombiePos.x() >= leftBound && zombiePos.x() <= rightBound && zombiePos.y() >= topBound && zombiePos.y() <= bottomBound)
                    //{
                        //existingZombies.at(j)->hide();
                        //existingZombies.at(j)->setAlive(false);
                    //}
                    if(existingZombies.at(j)->collidesWithItem(existingPlants.at(i)))
                    {
                        existingZombies.at(j)->hide();
                        existingZombies.at(j)->setAlive(false);
                        //
                        existingPlants.at(i)->hide();
                        existingPlants.at(i)->setAlive(false);
                        existingPlants.at(i)->setPos(2000,existingPlants.at(i)->pos().y()); // Removing it from view
                        existingPlants.at(i)->getLifeLabel()->setPos(2000,existingPlants.at(i)->pos().y()); // Removing it from view
                    }
                }
                break;
            }
            case 6:
            {
                qDebug() << "new snow projectile";
                Projectile *projectile = new Projectile;
                QPixmap projectilePixmap(":/pvz images/snowpeaprojectile.png");
                //projectilePixmap = projectilePixmap.scaledToWidth(10);
                projectile->setPixmap(projectilePixmap);
                scene->addItem(projectile);
                projectile->setPos(existingPlants.at(i)->pos().x()+40, existingPlants.at(i)->pos().y());
                projectile->setSlow(true); // because snow pea
                projectiles.push_back(projectile);
                qDebug()<<QString::fromStdString(existingPlants.at(i)->getName()) << "shoots";
                break;
            }
            case 7: // need to fix
            {
                if(existingPlants.at(i)->getChompReady())
                {
                    QPointF plantPos = existingPlants.at(i)->pos();
                    int leftBound = plantPos.x() + lawnWidth/9;
                    int rightBound = plantPos.x() + 2*lawnWidth/9;
                    int topBound = plantPos.y();
                    int bottomBound = plantPos.y() - lawnHeight/5;
                    for (int j=0; j<int(existingZombies.size()); j++)
                    {
                        QPointF zombiePos = existingZombies.at(j)->pos();
                        if(zombiePos.x() >= leftBound && zombiePos.x() <= rightBound && zombiePos.y() >= topBound && zombiePos.y() <= bottomBound)
                        {
                            existingZombies.at(j)->hide();
                            existingZombies.at(j)->setAlive(false);
                        }

                    }
                }
                //existingPlants.at(i)->hide();
                //existingPlants.at(i)->setAlive(false);
                //existingPlants.at(i)->setPos(2000,existingPlants.at(i)->pos().y()); // Removing it from view
                //existingPlants.at(i)->getLifeLabel()->setPos(2000,existingPlants.at(i)->pos().y()); // Removing it from view
                break;
            }
            case 8: // repeater
            {
                for (int k=0; k<2; k++) // 2 bullets for repeater
                {
                    qDebug() << "new projectile" << k;
                    Projectile *projectile = new Projectile;
                    QPixmap projectilePixmap(":/pvz images/peashooterprojectile.png");
                    //projectilePixmap = projectilePixmap.scaledToWidth(10);
                    projectile->setPixmap(projectilePixmap);
                    scene->addItem(projectile);
                    //distinguish the 2 bullets by a slight shift in x position
                    projectile->setPos(existingPlants.at(i)->pos().x()+40-k*10, existingPlants.at(i)->pos().y());

                    projectiles.push_back(projectile);
                    qDebug()<<QString::fromStdString(existingPlants.at(i)->getName()) << "shoots";

                }
                break;
            }
            }
        }
    }
}

void MainWindow::plantItemChecker()
{
    // checking projectiles
    //int numExistingProjectiles = projectiles.size(); // so loop doesn't go out of bounds if size changes
    //int numExistingZombies = existingZombies.size();

    // Collision detection

    for (int j=0; j<int(existingZombies.size()); j++)
    {
        if(existingZombies.at(j)->getAlive())
        {
            //int numExistingProjectiles = projectiles.size();
            for(int i=0; i<int(projectiles.size()); i++)
            {

                if(existingZombies.at(j)->collidesWithItem(projectiles.at(i))) // We have a collision
                {
                    //qDebug() << "projectile hit";

                    if (existingZombies.at(j)->getLife() <= 0)
                    {
                        //qDebug() << "before zombie delete";
                            existingZombies.at(j)->hide();
                            existingZombies.at(j)->setAlive(false);

        //                delete existingZombies.at(j);
        //                existingZombies.erase(existingZombies.begin()+j);
                        //qDebug() << "after zombie delete";
                    }
                    else
                    {
                        if(projectiles.at(i)->getSlow()) // If snow pea projectile
                        {
                            if(!existingZombies.at(j)->getSlowed()) // If zombie is not already slowed
                            {
                                existingZombies.at(j)->setSlowed(true);
                                existingZombies.at(j)->setSpeed(0.5*existingZombies.at(j)->getSpeed());
                            }
                        }
                        existingZombies.at(j)->setLife(existingZombies.at(j)->getLife() - 1);
                    }
                    //qDebug() << "before projectiles deleted";
                    projectiles.at(i)->hide();
                    projectiles.at(i)->setPos(2000,projectiles.at(i)->pos().y());
                    //delete projectiles.at(i);
                    //projectiles.erase(projectiles.begin()+i);
                    //qDebug() << "after projectiles deleted";

                    //qDebug() << "collision";
                }
            //if (projectiles.at(i))
            }


        for(int k=0; k<int(existingPlants.size()); k++)
        {
            if(existingPlants.at(k)->getAlive())
            {
                if (existingZombies.at(j)->collidesWithItem(existingPlants.at(k)))
                {
                    existingZombies.at(j)->setCollide(true);

                    if(!existingPlants.at(k)->getCollide())
                    {
                        existingPlants.at(k)->setCollide(true);
                        existingPlants.at(k)->addCollideZombie(existingZombies.at(j));
                    }
                    else
                    {
                        vector <Zombie *> existingCollideZombies = existingPlants.at(k)->getCollideZombies();

                        Zombie * theEndCollideZombie = existingCollideZombies.at(existingCollideZombies.size() -1);

                        // If last colliding zombie in vector is not the same as current colliding zombie
                        if (existingZombies.at(j)!= theEndCollideZombie)
                        {
                            existingPlants.at(k)->addCollideZombie(existingZombies.at(j)); // Add zombie
                        }

                    }

                    // If plant is dead
                    if (existingPlants.at(k)->getLife()<=0)
                    {
                        // Hide plant, set as dead
                        existingPlants.at(k)->hide();
                        existingPlants.at(k)->setAlive(false);
                        existingPlants.at(k)->setPos(2000,existingPlants.at(k)->pos().y()); // Removing it from view
                        existingPlants.at(k)->getLifeLabel()->setPos(2000,existingPlants.at(k)->pos().y()); // Removing it from view

                        vector <Zombie *> existingCollideZombies = existingPlants.at(k)->getCollideZombies();

                        // If plant is colliding
                        if (existingPlants.at(k)->getCollide())
                        {
                            for(int m=0; m <int(existingCollideZombies.size()); m++)
                            {
                                Zombie * theCollideZombie = existingCollideZombies.at(m);
                                if (theCollideZombie!= NULL)
                                {
                                    theCollideZombie->setCollide(false);
                                }
                            }
                        }
                    }
                }
            }
          }
        }
    }

    // Damaging the plant as zombie is colliding
    for(int i=0; i<int(existingPlants.size()); i++)
    {
        if (existingPlants.at(i) != NULL)
        {
            if (existingPlants.at(i)->getCollide()) // If plant is colliding with zombie
            {
                 vector <Zombie *> existingCollideZombies = existingPlants.at(i)->getCollideZombies();
                 for(int j=0; j <int(existingCollideZombies.size()); j++)
                 {
                    Zombie * collideZombie = existingCollideZombies.at(j);
                    if (collideZombie!= NULL && collideZombie->getAlive())
                    {
                        double plantLife = existingPlants.at(i)->getLife()-collideZombie->getAttackRate()*50/1000;
                        existingPlants.at(i)->setLife(plantLife);

                        if (plantLife>=0)
                        {
                            existingPlants.at(i)->getLifeLabel()->setPlainText("HP:" + QString::number(plantLife));
                        }
                        /*QGraphicsTextItem * lifeLabel = new QGraphicsTextItem;
                        lifeLabel->setPlainText("HP:" + QString::number(plantLife));
                        scene->addItem(lifeLabel);
                        lifeLabel->setPos(existingPlants.at(i)->pos().x(),existingPlants.at(i)->pos().y()-10);*/
                    }
                 }
            }
        }
    }

    // Checking plantSuns
    /*for(int i=0; i<int(plantSuns.size()); i++)
    {
        if(plantSuns.at(i)->getDuration() >= 7500) // If sun's duration has exceeded 7.5s, it will be deleted
        {
            delete plantSuns.at(i);
            plantSuns.erase(plantSuns.begin()+i);
        }
        else if(plantSuns.at(i)->getDeleteReady()) // Otherwise, if the sun is clicked, it will also be deleted
        {
            sunPoints += 25;
            delete plantSuns.at(i);
            plantSuns.erase(plantSuns.begin()+i);
            qDebug() << "sun deleted";
        }
    }*/

}

/*void MainWindow::plantItemChecker()
{
    // checking projectiles
    //int numExistingProjectiles = projectiles.size(); // so loop doesn't go out of bounds if size changes
    //int numExistingZombies = existingZombies.size();



    for(int i=0; i<int(projectiles.size()); i++)
    {
        if (projectiles.at(i)->pos().x() >= ui->graphicsView->width()+100)
        {
            delete projectiles.at(i);
            projectiles.erase(projectiles.begin()+i);
            //qDebug() <<"projectile deleted";
        }

        // Collision detection

        for (int j=0; j<int(existingZombies.size()); j++)
        {
            if(projectiles.at(i)->collidesWithItem(existingZombies.at(j))) // We have a colission
            {
                qDebug() << "projectile hit";

                if (existingZombies.at(j)->getLife() <= 0)
                {
                    qDebug() << "before zombie delete";
                    delete existingZombies.at(j);
                    existingZombies.erase(existingZombies.begin()+j);
                    qDebug() << "after zombie delete";
                }
                else
                {
                    qDebug() << "before zombie damaged";
                    existingZombies.at(j)->setLife(existingZombies.at(j)->getLife() - 1);
                }
                qDebug() << "before projectiles deleted";
                qDebug() << projectiles.size();
                delete projectiles.at(i);
                projectiles.erase(projectiles.begin()+i);
                qDebug() << projectiles.size();
                qDebug() << "after projectiles deleted";

                //qDebug() << "collision";
            }
        //if (projectiles.at(i))
        }

    }

    //checking plant/zombie collision
    //int numExistingPlants = existingPlants.size(); // so loop doesn't go out of bounds if size changes
    //numExistingZombies = existingZombies.size(); // size may have changed depending on above loop
    //int numExistingZombies = existingZombies.size();
    /for (int i=0; i<int(existingPlants.size()); i++)
    {
        for (int j=0; j<int(existingZombies.size()); j++)
        {
            if(existingZombies.at(j)->collidesWithItem(existingPlants.at(i))) // If zombie j colllides with plant i
            {
                existingZombies.at(j)->setCollide(true);

                if(!existingZombies.at(j)->isTimerStarted()) // if collision timer has not been started
                {
                    existingZombies.at(j)->startCollisionTimer(); // start the timer
                    qDebug() << "collision timer started";
                }

                if(existingZombies.at(j)->getCollisionTime()%1000 == 0) // every 2 seconds
                {
                    qDebug() << "plant health: "<<existingPlants.at(i)->getLife()-existingZombies.at(j)->getAttack();
                    existingPlants.at(i)->setLife(existingPlants.at(i)->getLife()-existingZombies.at(j)->getAttack()); // existing plant's life-1
                }

                int currentPlantLife = existingPlants.at(i)->getLife();
                if(currentPlantLife <= 0)
                {
                    for (int k=0; k<int(existingZombies.size()); k++) // checking which zombies are currently colliding wth currentplant
                    {
                        if(existingZombies.at(k)->collidesWithItem(existingPlants.at(i)))
                        {
                            existingZombies.at(k)->setCollide(false); // collide = false so zombie continues moving
                        }
                    }

                    qDebug() << "before delete";
                    delete existingPlants.at(i);
                    existingPlants.erase(existingPlants.begin()+i);
                    qDebug () << "deleted";

                    continue; // if deletes, then we go check the second plant
                }

            }
        }
    }/

    /for (int i =0; i<int(existingZombies.size()); i++)
    {
        for(int j=0; j<int(existingPlants.size()); j++)
        {

            if (existingZombies.at(i)->collidesWithItem(existingPlants.at(j))) // existingZombies.at(i)->pos().x() <= existingPlants.at(j)->pos().x())//
            {
                existingZombies.at(i)->setCollide(true); // stops zombie from moving

                //if (!existingZombies.at(i)->isTimerStarted()) // starts collision timer - uneven timing due to lag
                //{existingZombies.at(i)->startCollisionTimer();}

                if (existingPlants.at(j)->getLife()<=0) // if plant dead
                {
                    // allow all zombies previously colliding with that plant to start moving
                    for (int k=0; k<int(existingZombies.size()); k++)
                    {
                        if (existingZombies.at(k)->collidesWithItem(existingPlants.at(j)))
                        {
                            existingZombies.at(k)->setCollide(false);
                        }
                    }

                    //delete and erase plant
                    delete existingPlants.at(j);
                    existingPlants.erase(existingPlants.begin()+j);

                    //continue; // exit out of 2nd for loop
                }
                else
                {
                    //existingPlants.at(j)->setLife(existingPlants.at(j)->getLife()-existingZombies.at(i)->getAttack()); // if plant still has life, zombie will damage it
                }

            }
        }
    }
}*/

void MainWindow::projectileChecker()
{
    for(int i=0; i<int(projectiles.size()); i++)
    {
        if (projectiles.at(i) != NULL )
        {
            if (projectiles.at(i)->pos().x() >= ui->graphicsView->width())
            {
                projectiles.at(i)->hide();
                projectiles.at(i)->setPos(2000,projectiles.at(i)->pos().y());
                //delete projectiles.at(i);
                //projectiles.erase(projectiles.begin()+i);
                //qDebug() <<"projectile deleted";
            }
        }
    }
}

void MainWindow::gameEndChecker()
{
    // Checking to see if any zombie has gotten to house
    for(int i=0; i<int(existingZombies.size()); i++)
    {
        if(existingZombies.at(i)->getAlive())
        {
            //if(existingZombies.at(i)->pos().x() < lawnLeft-10) // if zombie has gotten to the beginning of the lawn/house
            //{
                for(int j=0; j<int(lawnmowers.size()); j++)
                {
                    if(existingZombies.at(i)->collidesWithItem(lawnmowers.at(j)))
                    {
                        lawnmowers.at(j)->setReady(true);

                    }
                }
            //}
        }
    }

    // Checking lawnmowers and "firing" them
    for(int i=0; i<int(lawnmowers.size()); i++)
    {
        if(lawnmowers.at(i)->getReady())
        {
            if(lawnmowers.at(i)->pos().x() > ui->graphicsView->width()+100)
            {
                lawnmowers.at(i)->hide();
                lawnmowers.at(i)->setReady(false);
                continue;
            }
            for(int j=0; j<int(existingZombies.size()); j++)
            {
                qDebug() << j << "th iteration";
                if(existingZombies.at(j)->getAlive())
                {
                    qDebug() << "before collide";
                    if(existingZombies.at(j)->collidesWithItem(lawnmowers.at(i)))
                    {
                        qDebug() << "before zombie delete";
                        existingZombies.at(j)->hide();
                        existingZombies.at(j)->setAlive(false);
                        qDebug() << "after zombie delete";
                    }
                }
            }
        }
    }

    for(int i=0; i<int(existingZombies.size()); i++)
    {
        if(existingZombies.at(i)->getAlive())
        {
            if(existingZombies.at(i)->pos().x() < lawnLeft-50) // if zombie has gotten to the beginning of the lawn/house
            {
                qDebug() << "GAME OVER";
                // Ending game/restarting level
                shootingTimer->stop();
                plantItemTimer->stop();
                sceneTimer->stop();
                zombieTimer->stop();
                sunTimer->stop();
                timer->stop();
                uiTimer->stop();
                projectileTimer->stop();
                gameEndTimer->stop();
                levelEndTimer->stop();

                QMessageBox msgBox;
                msgBox.setWindowTitle("GAME OVER");
                msgBox.setText("GAME OVER: Zombies have eaten your brain! Restart?");
                msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::No);

                if(msgBox.exec() == QMessageBox::Yes)
                {
                    // Deleting timers
                    delete shootingTimer;
                    delete plantItemTimer;
                    delete sceneTimer;
                    delete zombieTimer;
                    delete sunTimer;
                    delete timer;
                    delete uiTimer;
                    delete projectileTimer;
                    delete gameEndTimer;
                    delete levelEndTimer;

                    // Deleting and clearing scene/existing objects
                    scene->clear();
                    existingPlants.clear();
                    suns.clear();
                    plantSuns.clear();
                    existingZombies.clear();
                    projectiles.clear();
                    lawnmowers.clear();

                    // Resetting time counters
                    zombieCounter = 0;
                    zombieTimeCounter = 0;
                    sunTimeCounter = 0;
                    sunPoints = 1000;

                    // Resetting progress bars
                    ui->progressBar_1->setValue(0);
                    ui->progressBar_2->setValue(0);
                    ui->progressBar_3->setValue(0);
                    ui->progressBar_4->setValue(0);
                    ui->progressBar_5->setValue(0);
                    ui->progressBar_6->setValue(0);
                    ui->progressBar_7->setValue(0);
                    ui->progressBar_8->setValue(0);

                    plantReady = false;
                    ui->graphicsView->setReady(false);

                    on_startButton_clicked(); // Starting game again
                }
                else
                {
                    qApp->quit();
                    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
                }
            }
        }
    }
}

void MainWindow::levelEndChecker()
{
    // Checking if all zombies are dead
    if(zombieCounter >= int(currentZombieSequence.size()))
    {
        levelEnd = true;
        for(int i=0; i<int(existingZombies.size()); i++)
        {
            if(existingZombies.at(i)->getAlive())
            {
                levelEnd = false;
            }
        }
    }

    if(levelEnd)
    {
        levelEnd = false; // Setting levelEnd back to false

        if(currentUser->getLevel() < 3)
        {
            qDebug() << "level set";
            currentUser->setLevel(currentUser->getLevel()+1); //Advance to the next level

            qDebug() << "before deleting timers";
            // Deleting timers
            delete shootingTimer;
            delete plantItemTimer;
            delete sceneTimer;
            delete zombieTimer;
            delete sunTimer;
            delete timer;
            delete uiTimer;
            delete projectileTimer;
            delete gameEndTimer;
            delete levelEndTimer;

            qDebug() << "before deleting scene";
            // Deleting and clearing scene/existing objects
            scene->clear();
            existingPlants.clear();
            suns.clear();
            plantSuns.clear();
            existingZombies.clear();
            projectiles.clear();
            lawnmowers.clear();

            // Resetting counters
            zombieCounter = 0;
            zombieTimeCounter = 0;
            sunTimeCounter = 0;
            sunPoints = 1000;

            // Resetting progress bars
            ui->progressBar_1->setValue(0);
            ui->progressBar_2->setValue(0);
            ui->progressBar_3->setValue(0);
            ui->progressBar_4->setValue(0);
            ui->progressBar_5->setValue(0);
            ui->progressBar_6->setValue(0);
            ui->progressBar_7->setValue(0);
            ui->progressBar_8->setValue(0);

            plantReady = false;
            ui->graphicsView->setReady(false);

            qDebug() << "before restarting level";
            on_startButton_clicked(); // Starting game again but from next level
        }
        else if(currentUser->getLevel() == 3) // If completed last level
        {
            shootingTimer->stop();
            plantItemTimer->stop();
            sceneTimer->stop();
            zombieTimer->stop();
            sunTimer->stop();
            timer->stop();
            uiTimer->stop();
            projectileTimer->stop();
            gameEndTimer->stop();
            levelEndTimer->stop();

            QMessageBox msgBox;
            msgBox.setWindowTitle("Congradulations!");
            msgBox.setText("You have beaten all the zombies!");
            msgBox.setStandardButtons(QMessageBox::Yes);
            msgBox.setButtonText(QMessageBox::Yes, QString("Sweet!"));
            msgBox.setDefaultButton(QMessageBox::Yes);

            if(msgBox.exec() == QMessageBox::Yes)
            {
                // Deleting timers
                delete shootingTimer;
                delete plantItemTimer;
                delete sceneTimer;
                delete zombieTimer;
                delete sunTimer;
                delete timer;
                delete uiTimer;
                delete projectileTimer;
                delete gameEndTimer;
                delete levelEndTimer;

                // Deleting and clearing scene/existing objects
                scene->clear();
                existingPlants.clear();
                suns.clear();
                plantSuns.clear();
                existingZombies.clear();
                projectiles.clear();
                lawnmowers.clear();

                currentUser->setLevel(1);

                QPixmap titlePage(":/pvz images/titlepage.png");
                titlePage = titlePage.scaledToHeight(ui->graphicsView->height());
                QGraphicsPixmapItem *pm = scene->addPixmap(titlePage);
                pm->setPos((ui->graphicsView->width()-titlePage.width())/2, 0);

                ui->userComboBox->setEnabled(1);
                ui->startButton->setEnabled(1);
                ui->deleteButton->setEnabled(1);
                ui->restartButton->setEnabled(0);

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
        if(suns.at(i)->getDuration() >= 3000)
        {
            //vector<Sun*>::iterator iter;
            //for(int j=0; j<i; iter++, j++);
            delete suns.at(i);
            suns.erase(suns.begin()+i);
        }
        else if(suns.at(i)->getDeleteReady())
        {
            sunPoints += 25;
            delete suns.at(i);
            suns.erase(suns.begin()+i);
            qDebug() << "deleted";
        }
    }

    if(sunTimeCounter%3000 == 0 && sunTimeCounter <= 0)
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
        suns.at(i)->setDuration(suns.at(i)->getDuration()+50);
    }
}*/

void MainWindow::on_p1Button_clicked()
{
    currentPlant = plants[0]; // Sets current plant pointer to selected plant in the plants vector
    plantReady = true;

    // If button has been pressed once, will disable plantReady.
    //Opposite because click signal will set checked and this will read the opposite of what clicked just set (which is the previous state
    if(!ui->p1Button->isChecked())
    {
        plantReady = false;
        setButtonsCheck(0); // Disables other buttons
        ui->p1Button->setChecked(0);
    }
    else // If button has not yet been pressed
    {
        setButtonsCheck(0); // Disable other buttons
        ui->p1Button->setChecked(1); // Checks this one
        //qDebug() << plantReady;
    }

}

void MainWindow::on_p2Button_clicked()
{
    currentPlant = plants[1];
    plantReady = true;

    if(!ui->p2Button->isChecked())
    {
        plantReady = false;
        setButtonsCheck(0); // Disables other buttons
        ui->p2Button->setChecked(0);
    }
    else // If button has not yet been pressed
    {
        setButtonsCheck(0); // Disable other buttons
        ui->p2Button->setChecked(1); // Checks this one
    }
}

void MainWindow::on_p3Button_clicked()
{
    currentPlant = plants[2];
    plantReady = true;

    if(!ui->p3Button->isChecked())
    {
        plantReady = false;
        setButtonsCheck(0); // Disables other buttons
        ui->p3Button->setChecked(0);
    }
    else // If button has not yet been pressed
    {
        setButtonsCheck(0); // Disable other buttons
        ui->p3Button->setChecked(1); // Checks this one
    }
}

void MainWindow::on_p4Button_clicked()
{
    currentPlant = plants[3];
    plantReady = true;

    if(!ui->p4Button->isChecked())
    {
        plantReady = false;
        setButtonsCheck(0); // Disables other buttons
        ui->p4Button->setChecked(0);
    }
    else // If button has not yet been pressed
    {
        setButtonsCheck(0); // Disable other buttons
        ui->p4Button->setChecked(1); // Checks this one
    }
}

void MainWindow::on_p5Button_clicked()
{
    currentPlant = plants[4];
    plantReady = true;

    if(!ui->p5Button->isChecked())
    {
        plantReady = false;
        setButtonsCheck(0); // Disables other buttons
        ui->p5Button->setChecked(0);
    }
    else // If button has not yet been pressed
    {
        setButtonsCheck(0); // Disable other buttons
        ui->p5Button->setChecked(1); // Checks this one
    }
}

void MainWindow::on_p6Button_clicked()
{
    currentPlant = plants[5];
    plantReady = true;

    if(!ui->p6Button->isChecked())
    {
        plantReady = false;
        setButtonsCheck(0); // Disables other buttons
        ui->p6Button->setChecked(0);
    }
    else // If button has not yet been pressed
    {
        setButtonsCheck(0); // Disable other buttons
        ui->p6Button->setChecked(1); // Checks this one
    }
}

void MainWindow::on_p7Button_clicked()
{
    currentPlant = plants[6];
    plantReady = true;

    if(!ui->p7Button->isChecked())
    {
        plantReady = false;
        setButtonsCheck(0); // Disables other buttons
        ui->p7Button->setChecked(0);
    }
    else // If button has not yet been pressed
    {
        setButtonsCheck(0); // Disable other buttons
        ui->p7Button->setChecked(1); // Checks this one
    }
}

void MainWindow::on_p8Button_clicked()
{
    currentPlant = plants[7];
    plantReady = true;

    if(!ui->p8Button->isChecked())
    {
        plantReady = false;
        setButtonsCheck(0); // Disables other buttons
        ui->p8Button->setChecked(0);
    }
    else // If button has not yet been pressed
    {
        setButtonsCheck(0); // Disable other buttons
        ui->p8Button->setChecked(1); // Checks this one
    }
}

void MainWindow::on_userComboBox_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->nameLineEdit->setText("User Name");
        ui->levelLineEdit->setText("No User Selected");
        ui->deleteButton->setEnabled(0);
        ui->startButton->setEnabled(0);

    }
    else if (index > 0) // For some reason the index is -1 and 0(?) at startup.
    {
        ui->deleteButton->setEnabled(1);
        ui->startButton->setEnabled(1);
        ui->newButton->setEnabled(0);
        ui->nameLineEdit->setText(QString::fromStdString(users[index-1]->getName()));
        ui->levelLineEdit->setText("Level: " + QString::number(users[index-1]->getLevel()));

        currentUser = users[index-1];
        //currentLevel = levels[index-1];
    }
    qDebug() << index;
}

void MainWindow::on_deleteButton_clicked()
{
    if(!users.empty())
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
    QString newUserName = ui->nameLineEdit->text();

    // Checking if user name is blank
    if(newUserName.isEmpty()) // If there is nothing in the name line edit, disable new user button
    {
        qDebug() << "Player name must not be empty.";
        return;
    }

    // Checking if user name is alphanumeric
    for (int j = 0; j < newUserName.size(); j++)
    {
        if (!newUserName.at(j).isLetterOrNumber())// && !fileList.at(1).at(j).isSpace())
        {
            qDebug() << "Player name must be alphanumeric and must contain no spaces.";
            return;
        }
    }

    // Checking if user name already exists
    for (int k = 0; k < int(users.size()); k++)
    {
        if (newUserName.toStdString() == users.at(k)->getName())
        {
            qDebug() << "Username already exists.";
            return;
        }
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("New User");
    msgBox.setText("Are you sure you want to create new user: " + newUserName + "?");
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if(msgBox.exec() == QMessageBox::Yes){
        User * newUser = new User;
        newUser->setName(newUserName.toStdString()); // Setting name of user to the typed name on nameLineEdit
        newUser->setLevel(1); // Setting level to 1 or else newUser gets random level.
        users.push_back(newUser);
        ui->userComboBox->addItem(QString::fromStdString(newUser->getName()), int(users.size())); // Adding user to the userComboBox
    }
    else
    {
        qDebug() << "User was not added.";
        return;
    }

}

void MainWindow::on_startButton_clicked()
{
    // Deletes previous background/stuff
    scene->clear();

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

    // Setting currentLevel and curentZombieSequence depending on the selected user when start was clicked
    currentLevel = levels[currentUser->getLevel()-1];
    currentZombieSequence = currentLevel->getZombieSequence();

    // Depending on user's current level, the playable grid and lawn will be different
    qDebug()<< "current level:" << currentLevel->getLevelNumber();

    QPixmap lawn;

    // Implementing a 5x9 grid that will be used later on for ease of operation
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<columns; j++)
        {
            int x = lawnLeft + j*lawnWidth/columns;
            int y = lawnTop + i*lawnHeight/rows; // To designate middle row
            QPointF gridPoint;
            gridPoint.setX(x);
            gridPoint.setY(y);
            grid.at(i).at(j) = gridPoint;
        }
    }

    if(currentLevel->getLevelNumber() == 1)
    {
        // Changing background to level 1 lawn
        lawn.load(":/pvz images/level1lawn.jpg");
        lawn = lawn.scaledToWidth(ui->graphicsView->width()); // Scaling lawn to graphicsview
        scene->addPixmap(lawn);

        Lawnmower * aLawnmower = new Lawnmower;
        QPixmap lawnmowerPixmap(":/pvz images/lawnmower.png");
        lawnmowerPixmap = lawnmowerPixmap.scaledToWidth(lawnWidth/columns);
        aLawnmower->setPixmap(lawnmowerPixmap);
        scene->addItem(aLawnmower);
        aLawnmower->setPos(lawnLeft-1*lawnWidth/columns, lawnTop+2*lawnHeight/rows+10);
        lawnmowers.push_back(aLawnmower);


    }
    else if(currentLevel->getLevelNumber() == 2)
    {
        // Changing background to level 2 lawn
        lawn.load(":/pvz images/level2lawn.jpg");
        lawn = lawn.scaledToWidth(ui->graphicsView->width()); // Scaling lawn to graphicsview
        scene->addPixmap(lawn);

        for (int i=0; i<3; i++)
        {
            Lawnmower * aLawnmower = new Lawnmower;
            QPixmap lawnmowerPixmap(":/pvz images/lawnmower.png");
            lawnmowerPixmap = lawnmowerPixmap.scaledToWidth(lawnWidth/columns);
            aLawnmower->setPixmap(lawnmowerPixmap);
            scene->addItem(aLawnmower);
            aLawnmower->setPos(lawnLeft-1*lawnWidth/columns, lawnTop+(i+1)*lawnHeight/rows+10);
            lawnmowers.push_back(aLawnmower);
        }
    }
    else if(currentLevel->getLevelNumber() == 3)
    {
        // Changing background to level 3 lawn
        lawn.load(":/pvz images/lawn.jpg");
        lawn = lawn.scaledToWidth(ui->graphicsView->width()); // Scaling lawn to graphicsview
        scene->addPixmap(lawn);

        for (int i=0; i<5; i++)
        {
            Lawnmower * aLawnmower = new Lawnmower;
            QPixmap lawnmowerPixmap(":/pvz images/lawnmower.png");
            lawnmowerPixmap = lawnmowerPixmap.scaledToWidth(lawnWidth/columns);
            aLawnmower->setPixmap(lawnmowerPixmap);
            scene->addItem(aLawnmower);
            aLawnmower->setPos(lawnLeft-1*lawnWidth/columns, lawnTop+(i)*lawnHeight/rows+10);
            lawnmowers.push_back(aLawnmower);
        }
    }

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<columns; j++)
        {
            qDebug() << grid.at(i).at(j);
        }
    }

    QRectF lawnRect(lawnLeft,lawnTop,lawnWidth,lawnHeight); // Creating a lawn rectangle with bounds left,up,width,height

    // Draw lines to help out with general visualization
    QPen my_pen = QPen(Qt::black);
    scene->addLine(QLineF(), my_pen);
    scene->addLine(QLineF(lawnRect.bottomLeft(), lawnRect.bottomRight()) ,my_pen);
    scene->addLine(QLineF(lawnRect.topLeft(), lawnRect.topRight()) ,my_pen);
    scene->addLine(QLineF(lawnRect.bottomLeft(), lawnRect.topLeft()) ,my_pen);
    scene->addLine(QLineF(lawnRect.topRight(), lawnRect.bottomRight()) ,my_pen);

    // Drawing a grid to help with visualization
    for (int i=-1; i<9; i++)
    {
        scene->addLine(lawnLeft+i*lawnWidth/9,lawnTop,lawnLeft+i*lawnWidth/9, lawnBottom); // drawing vertical
    }
    for (int i=0; i<6; i++)
    {
        scene->addLine(lawnLeft-lawnWidth/9,lawnTop+i*lawnHeight/5,lawnRight,lawnTop+i*lawnHeight/5); // drawing horizontal
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

    // Set up a timer which will periodically allow the plants to shoot based on each plants fireRate
    shootingTimer = new QTimer(this);
    connect(shootingTimer, SIGNAL(timeout()), this, SLOT(plantShooter()));
    shootingTimer->start(50);

    // Set up a timer which will periodically check the plant items such as sun drops, projectiles, etc.
    plantItemTimer = new QTimer(this);
    connect(plantItemTimer, SIGNAL(timeout()), this, SLOT(plantItemChecker()));
    plantItemTimer->start(50);

    // Set up a timer which will periodically check the plant items such as sun drops, projectiles, etc.
    projectileTimer = new QTimer(this);
    connect(projectileTimer, SIGNAL(timeout()), this, SLOT(projectileChecker()));
    projectileTimer->start(50);

    // Set up a timer which will periodically check the plant items such as sun drops, projectiles, etc.
    gameEndTimer = new QTimer(this);
    connect(gameEndTimer, SIGNAL(timeout()), this, SLOT(gameEndChecker()));
    gameEndTimer->start(50);

    levelEndTimer = new QTimer(this);
    connect(levelEndTimer, SIGNAL(timeout()), this, SLOT(levelEndChecker()));
    levelEndTimer->start(50);

    plantReady = false;
    ui->graphicsView->setReady(false);

    qDebug() << "GAME START";
}

void MainWindow::on_restartButton_clicked()
{
    shootingTimer->stop();
    plantItemTimer->stop();
    sceneTimer->stop();
    zombieTimer->stop();
    sunTimer->stop();
    timer->stop();
    uiTimer->stop();
    projectileTimer->stop();
    gameEndTimer->stop();
    levelEndTimer->stop();

    QMessageBox msgBox;
    msgBox.setWindowTitle("Restart");
    msgBox.setText("Are you sure you want to restart the level?");
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes)
    {
        // Deleting all current objects

        /*for(int i=0; i<int(existingPlants.size() && !existingPlants.empty()); i++)
        {
            delete existingPlants.at(i);
        }
        for(int j=0; j<int(existingZombies.size()); j++)
        {
            delete existingZombies.at(j);
        }
        for(int k=0; k<int(projectiles.size()); k++)
        {
            delete projectiles.at(k);
        }
        for(int l=0; l<int(suns.size()); l++)
        {
            delete suns.at(l);
        }
        for(int m=0; m<int(plantSuns.size()); m++)
        {
            delete plantSuns.at(m);
        }
*/
        delete shootingTimer;
        delete plantItemTimer;
        delete sceneTimer;
        delete zombieTimer;
        delete sunTimer;
        delete timer;
        delete uiTimer;
        delete projectileTimer;
        delete gameEndTimer;
        delete levelEndTimer;

        // Deleting and clearing scene/existing objects
        scene->clear();
        existingPlants.clear();
        suns.clear();
        plantSuns.clear();
        existingZombies.clear();
        projectiles.clear();
        lawnmowers.clear();

        // Resetting time counters
        zombieCounter = 0;
        zombieTimeCounter = 0;
        sunTimeCounter = 0;
        sunPoints = 1000;

        // Resetting progress bars
        ui->progressBar_1->setValue(0);
        ui->progressBar_2->setValue(0);
        ui->progressBar_3->setValue(0);
        ui->progressBar_4->setValue(0);
        ui->progressBar_5->setValue(0);
        ui->progressBar_6->setValue(0);
        ui->progressBar_7->setValue(0);
        ui->progressBar_8->setValue(0);

        plantReady = false;
        ui->graphicsView->setReady(false);

        on_startButton_clicked(); // Starting game again
    }
    else
    {
        shootingTimer->start(50);
        plantItemTimer->start(50);
        sceneTimer->start(50);
        zombieTimer->start(50);
        sunTimer->start(50);
        timer->start(50);
        uiTimer->start(50);
        projectileTimer->start(50);
        gameEndTimer->start(50);
        levelEndTimer->start(50 );
        return;
    }



}
