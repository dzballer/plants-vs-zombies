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
    currentZombie(NULL), plantReady(false), sunPoints(100), sunTimeCounter(0), zombieCounter(0),
    zombieTimeCounter(0), lastZombieSpawnTime(0), zombieInterval(5000), rows(5), columns(9), grid(rows, std::vector<QPointF>(columns)), levelEnd(false)
{
    qsrand(QTime::currentTime().msec());

    ui->setupUi(this);

    // Creating gameManager that loads/reads files and data
    gameManager = new GameManager();

    // Setting progressbar ranges for seeding times
    ui->progressBar_1->setRange(0,7500);
    ui->progressBar_2->setRange(0,7500);
    ui->progressBar_3->setRange(0,50000);
    ui->progressBar_4->setRange(0,30000);
    ui->progressBar_5->setRange(0,30000);
    ui->progressBar_6->setRange(0,7500);
    ui->progressBar_7->setRange(0,7500);
    ui->progressBar_8->setRange(0,7500);

    // Setting all progress bars to max
    ui->progressBar_1->setValue(ui->progressBar_1->maximum());
    ui->progressBar_2->setValue(ui->progressBar_2->maximum());
    ui->progressBar_3->setValue(ui->progressBar_3->maximum());
    ui->progressBar_4->setValue(ui->progressBar_4->maximum());
    ui->progressBar_5->setValue(ui->progressBar_5->maximum());
    ui->progressBar_6->setValue(ui->progressBar_6->maximum());
    ui->progressBar_7->setValue(ui->progressBar_7->maximum());
    ui->progressBar_8->setValue(ui->progressBar_8->maximum());

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
    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setStyleSheet("background: black");
    scene->setSceneRect(0,0,ui->graphicsView->width()-2,ui->graphicsView->height()-2);;
    QPixmap titlePage(":/pvz images/titlepage.png");
    titlePage = titlePage.scaledToHeight(ui->graphicsView->height());
    QGraphicsPixmapItem *pm = scene->addPixmap(titlePage);
    pm->setPos((ui->graphicsView->width()-titlePage.width())/2, 0);

    // Sorting users by timestamp using bubblesort
    sortComboBox();

    // Adding users
    ui->userComboBox->clear();
    ui->userComboBox->addItem("Select/Create User"); // Setting initial ComboBox label to "Users" with an irrelevant index

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

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(!ui->graphicsView->getReady())
        plantReady = false; // If user clicks anywhere outside of graphicsview, the plantReady will be terminated
    setButtonsCheck(0);
    QMainWindow::mousePressEvent(e);
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

void MainWindow::saveFile(vector<User *> user_vector, User *current_user)
{
    QFile save_file(":/pvz_players.csv");

    current_user->setTimeStamp(QDateTime::currentDateTime().toTime_t()); // Retrieving and updating timestamp to the current user that is passed to the function

    if (save_file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {
         QTextStream out(&save_file);
         for (int i = 0; i<int(user_vector.size()); i++)
         {
             out << user_vector[i]->getTimeStamp() << ":" << user_vector[i]->getName().c_str() << ":" << user_vector[i]->getLevel() << "\n";
         }
         qDebug() << QString::fromStdString(currentUser->getName()) << "data saved";
         save_file.close();
    }
    else
    {
        qDebug() << "Error: Unable to save file.\n";
    }
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
    if(sunPoints < plants[0]->getCost() || ui->progressBar_1->value() < .99*ui->progressBar_1->maximum()) // Progress bar only goes up to ~99% of maximum
    {
        ui->p1Button->setEnabled(0);
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
    }

    if (ui->levelLineEdit->text().toInt() != currentUser->getLevel())
    {
        ui->levelLineEdit->setText("Level: " + QString::number(currentUser->getLevel()));
    }
}

// Runs on a timer and checks if two boolean conditions are met. Draws pixmap and adds to scene if conditions are satisfied.
void MainWindow::drawPlantChecker()
{
    // If plant has been selected, below statement will turn true when user clicks the lawn
    if (plantReady == true && ui->graphicsView->getReady())
    {
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
        else if(currentLevel->getLevelNumber() >= 3) // If level 3+
        {
            for(int i = 0; i<5; i++) // Checking 5 rows
            {
                for(int j = 0; j<9; j++)
                {
                    if(itemPos.y() < grid[0].at(j).y() || itemPos.y() > grid[4].at(j).y())  // Checking full lawn
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

        plantReady = false;
        setButtonsCheck(0);

        // Taking away sunpoints
        sunPoints -= currentPlant->getCost();

        // Resets progress bar timer when current plant is planted
        if(currentPlant->getIndex() == 1)
        {
            ui->progressBar_1->setValue(0);
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
    // Only starts creating zombies when the level start time is reached (in seconds)
    if(zombieTimeCounter >= currentLevel->getStartTime()*1000)
    {
        if(zombieCounter >= int(currentZombieSequence.size()))
        {
            zombieTimer->stop();
            return;
        }

        if(zombieInterval == 0)
        {
            zombieInterval = currentLevel->getInterval()*1000;
        }

        double timeSinceLastSpawn = zombieTimeCounter-lastZombieSpawnTime;

        // If time since last spawn coincides with zombie interval
        if(fmod((timeSinceLastSpawn),(zombieInterval)) == 0)
        {
            if (currentLevel->getLevelNumber() == 1)
            {
                // Creating Zombies
                currentZombie = zombies[currentZombieSequence[zombieCounter]-1]; //Using to hold current zombie
                Zombie * aZombie = new Zombie(currentZombie); // Need to new everytime or else same current zombie is added to scene

                QPixmap zombie(":/pvz images/" + QString::fromStdString(currentZombie->getName()) + ".png");

                zombie = zombie.scaledToHeight(lawnHeight/5); // More appropriate than width based on zombie height variation
                aZombie->setPixmap(zombie);
                scene->addItem(aZombie);
                aZombie->setPos(ui->graphicsView->width(),lawnTop+2*lawnHeight/(5)); //Middle row

                existingZombies.push_back(aZombie);

                // Implementing interval decrement
                if(zombieInterval-currentLevel->getIntervalDecrement()*1000 >= 0)
                {
                    zombieInterval -= currentLevel->getIntervalDecrement()*1000;
                }
                lastZombieSpawnTime = zombieTimeCounter;
                zombieCounter++;
            }
            else if(currentLevel->getLevelNumber() == 2)
            {
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

                // Implementing interval decrement
                if(zombieInterval-currentLevel->getIntervalDecrement()*1000 >= 0)
                {
                    zombieInterval -= currentLevel->getIntervalDecrement()*1000;
                }
                lastZombieSpawnTime = zombieTimeCounter;
                zombieCounter++;
            }
            else if(currentLevel->getLevelNumber() >= 3)
            {
                // Creating Zombies
                currentZombie = zombies[currentZombieSequence[zombieCounter]-1]; //Using to hold current zombie
                Zombie * aZombie = new Zombie(currentZombie); // Need to new everytime or else same current zombie is added to scene

                QPixmap zombie(":/pvz images/" + QString::fromStdString(currentZombie->getName()) + ".png");

                zombie = zombie.scaledToHeight(lawnHeight/5);
                aZombie->setPixmap(zombie);
                scene->addItem(aZombie);
                aZombie->setPos(ui->graphicsView->width()-30,lawnTop+(qrand()%5)*lawnHeight/5); // Random row
                existingZombies.push_back(aZombie);

                // Implementing interval decrement
                if(zombieInterval-currentLevel->getIntervalDecrement()*1000 >= 0)
                {
                    zombieInterval -= currentLevel->getIntervalDecrement()*1000;
                }
                lastZombieSpawnTime = zombieTimeCounter;
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
        else if(currentLevel->getLevelNumber() >= 3)
        {
            randRow = qrand()%5; // Rows 1-5
            randColumn = qrand()%(columns);
        }
        else
        {
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
        if (suns.at(i)->getFinalPos().y() <= suns.at(i)->pos().y()+5) // Once sun is at rest
        {
            suns.at(i)->setDuration(suns.at(i)->getDuration()+50); // Adds to each sun's timeCounter/duration
        }
    }
}

void MainWindow::plantShooter()
{
    for(int i=0; i<int(existingPlants.size()) && !existingPlants.empty(); i++)
    {
        if(existingPlants.at(i)->getAlive() && fmod((existingPlants.at(i)->getShootingTimeCounter()),(1000*existingPlants.at(i)->getFireRate())) == 0 && existingPlants.at(i)->getShootingTimeCounter()!= 0 )
        {
            switch (existingPlants.at(i)->getIndex())
            {
            default:
                break;

            case 1:
            {
                Projectile *projectile = new Projectile;
                QPixmap projectilePixmap(":/pvz images/peashooterprojectile.png");
                projectile->setPixmap(projectilePixmap);
                scene->addItem(projectile);
                projectile->setPos(existingPlants.at(i)->pos().x()+40, existingPlants.at(i)->pos().y());

                projectiles.push_back(projectile);
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
            case 5: // Potato mine
            {
                existingPlants.at(i)->setBombReady(true);
                break;
            }
            case 6:
            {
                Projectile *projectile = new Projectile;
                QPixmap projectilePixmap(":/pvz images/snowpeaprojectile.png");
                projectile->setPixmap(projectilePixmap);
                scene->addItem(projectile);
                projectile->setPos(existingPlants.at(i)->pos().x()+40, existingPlants.at(i)->pos().y());
                projectile->setSlow(true); // because snow pea
                projectiles.push_back(projectile);
                break;
            }
            case 7: // chomper
            {
                existingPlants.at(i)->setChompReady(true);
                break;
            }
            case 8: // repeater
            {
                for (int k=0; k<2; k++) // 2 bullets for repeater
                {
                    Projectile *projectile = new Projectile;
                    QPixmap projectilePixmap(":/pvz images/peashooterprojectile.png");
                    projectile->setPixmap(projectilePixmap);
                    scene->addItem(projectile);
                    //distinguish the 2 bullets by a slight shift in x position
                    projectile->setPos(existingPlants.at(i)->pos().x()+40-k*10, existingPlants.at(i)->pos().y());

                    projectiles.push_back(projectile);
                }
                break;
            }
            }
        }
    }
}

void MainWindow::plantItemChecker()
{
    // Checking for chomper plant
    for(int j=0; j<int(existingPlants.size()); j++)
    {
        if(existingPlants.at(j)->getChompReady())
        {
            QPointF plantPos = existingPlants.at(j)->pos();
            int leftBound = plantPos.x();
            int rightBound = plantPos.x() + 2*lawnWidth/9;
            int topBound = plantPos.y();
            int bottomBound = plantPos.y() - lawnHeight/5;
            for (int k=0; k<int(existingZombies.size()); k++)
            {

                QPointF zombiePos = existingZombies.at(k)->pos();
                if(zombiePos.x() >= leftBound && zombiePos.x() <= rightBound && zombiePos.y() <= topBound && zombiePos.y() >= bottomBound) // giving room for error
                {
                    existingZombies.at(k)->hide();
                    existingZombies.at(k)->setAlive(false);
                    existingPlants.at(j)->setChompReady(false); // setting it back to false for cooldown

                    break; // can only eat 1 plant
                }

            }
        }
    }


    // Collision detection for projectile and zombies
    for (int j=0; j<int(existingZombies.size()); j++)
    {
        if(existingZombies.at(j)->getAlive())
        {
            for(int i=0; i<int(projectiles.size()); i++)
            {

                if(existingZombies.at(j)->collidesWithItem(projectiles.at(i))) // We have a collision
                {
                    if (existingZombies.at(j)->getLife() <= 1) // Or else hits 11 times
                    {
                            existingZombies.at(j)->hide();
                            existingZombies.at(j)->setAlive(false);
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

                    projectiles.at(i)->hide();
                    projectiles.at(i)->setPos(2000,projectiles.at(i)->pos().y());
                }
            }


        // Checking plants and zombie collision
        for(int k=0; k<int(existingPlants.size()); k++)
        {
            if(existingPlants.at(k)->getAlive())
            {
                if (existingZombies.at(j)->collidesWithItem(existingPlants.at(k)))
                {
                    if(existingPlants.at(k)->getIndex() == 5) // If potatomine
                    {
                        if(existingPlants.at(k)->getBombReady())
                        {
                            existingZombies.at(j)->hide();
                            existingZombies.at(j)->setAlive(false);
                            existingPlants.at(k)->hide();
                            existingPlants.at(k)->setAlive(false);
                            existingPlants.at(k)->setPos(2000,existingPlants.at(k)->pos().y()); // Removing it from view
                            existingPlants.at(k)->getLifeLabel()->setPos(2000,existingPlants.at(k)->pos().y()); // Removing it from view
                            existingPlants.at(k)->setBombReady(false);
                        }
                    }

                    if(existingPlants.at(k)->getIndex() != 3)
                    existingZombies.at(j)->setCollide(true); // Zombie stops moving as long as not cherry bomb

                    if(!existingPlants.at(k)->getCollide())
                    {
                        existingPlants.at(k)->setCollide(true);
                        existingPlants.at(k)->addCollideZombie(existingZombies.at(j));
                    }
                    else
                    {
                        vector <Zombie *> existingCollideZombies = existingPlants.at(k)->getCollideZombies();

                        bool isExisting = false;
                        for (int n = 0; n<int(existingCollideZombies.size()); n++)
                        {
                            Zombie * theEndCollideZombie = existingCollideZombies.at(n);
                            // If last colliding zombie in vector is not the same as current colliding zombie
                            if (existingZombies.at(j) == theEndCollideZombie)
                            {
                                isExisting = true;
                                break;
                            }
                        }
                        if (!isExisting)
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

                        // showing zombie attack - WIP
                        /*QGraphicsTextItem * attackLabel = new QGraphicsTextItem;
                        attackLabel->setPlainText("Attacking!");
                        scene->addItem(attackLabel);
                        attackLabel->setPos(collideZombie->pos().x(),collideZombie->pos().y()-qrand()%15);
                        attackLabel->setDefaultTextColor(Qt::red);
                        textItems.push_back(attackLabel);*/

                        if (plantLife>=0)
                        {
                            existingPlants.at(i)->getLifeLabel()->setPlainText("HP:" + QString::number(int(plantLife)));
                        }
                    }
                 }
            }
        }
    }

    // Checking plantSuns
    for(int i=0; i<int(plantSuns.size()); i++)
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
        }
        else if (plantSuns.at(i)->getFinalPos().y() <= plantSuns.at(i)->pos().y()+5) // Once sun is at rest
        {
            plantSuns.at(i)->setDuration(plantSuns.at(i)->getDuration()+50); // Adds to each sun's timeCounter/duration
        }
    }

}

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
            for(int j=0; j<int(lawnmowers.size()); j++)
            {
                if(existingZombies.at(i)->collidesWithItem(lawnmowers.at(j)))
                {
                    lawnmowers.at(j)->setReady(true);

                }
            }
        }
    }

    // Checking lawnmowers and "firing" them
    for(int i=0; i<int(lawnmowers.size()); i++)
    {
        if(lawnmowers.at(i)->getReady())
        {
            if(lawnmowers.at(i)->pos().x() > ui->graphicsView->width()+100) // When lawnmower goes out of range
            {
                lawnmowers.at(i)->hide();
                lawnmowers.at(i)->setReady(false);
                continue;
            }
            for(int j=0; j<int(existingZombies.size()); j++)
            {
                if(existingZombies.at(j)->getAlive())
                {
                    if(existingZombies.at(j)->collidesWithItem(lawnmowers.at(i))) // When lawnmower collides with zombies
                    {
                        existingZombies.at(j)->hide();
                        existingZombies.at(j)->setAlive(false);
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
                    zombieInterval = 0;
                    lastZombieSpawnTime = 0;
                    zombieCounter = 0;
                    zombieTimeCounter = 0;
                    sunTimeCounter = 0;
                    sunPoints = 1000;

                    // Resetting plant buttons
                    setButtonsCheck(0);

                    // Resetting progress bars
                    ui->progressBar_1->setValue(ui->progressBar_1->maximum());
                    ui->progressBar_2->setValue(ui->progressBar_2->maximum());
                    ui->progressBar_3->setValue(ui->progressBar_3->maximum());
                    ui->progressBar_4->setValue(ui->progressBar_4->maximum());
                    ui->progressBar_5->setValue(ui->progressBar_5->maximum());
                    ui->progressBar_6->setValue(ui->progressBar_6->maximum());
                    ui->progressBar_7->setValue(ui->progressBar_7->maximum());
                    ui->progressBar_8->setValue(ui->progressBar_8->maximum());

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

        if(currentUser->getLevel() < levels.at(levels.size()-1)->getLevelNumber())
        {
            currentUser->setLevel(currentUser->getLevel()+1); //Advance to the next level

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

            // Resetting counters
            zombieInterval = 0;
            lastZombieSpawnTime = 0;
            zombieCounter = 0;
            zombieTimeCounter = 0;
            sunTimeCounter = 0;
            sunPoints = 1000;

            // Resetting progress bars
            ui->progressBar_1->setValue(ui->progressBar_1->maximum());
            ui->progressBar_2->setValue(ui->progressBar_2->maximum());
            ui->progressBar_3->setValue(ui->progressBar_3->maximum());
            ui->progressBar_4->setValue(ui->progressBar_4->maximum());
            ui->progressBar_5->setValue(ui->progressBar_5->maximum());
            ui->progressBar_6->setValue(ui->progressBar_6->maximum());
            ui->progressBar_7->setValue(ui->progressBar_7->maximum());
            ui->progressBar_8->setValue(ui->progressBar_8->maximum());

            // Reset plant buttons
            setButtonsCheck(0);

            plantReady = false;
            ui->graphicsView->setReady(false);

            on_startButton_clicked(); // Starting game again but from next level
        }
        else if(currentUser->getLevel() >= levels.at(levels.size()-1)->getLevelNumber()) // If completed last level
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
            msgBox.setWindowTitle("Congratulations!");
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

                uiUpdater();
            }
        }
    }
}

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
        ui->newButton->setEnabled(1);
        ui->deleteButton->setEnabled(0);
        ui->startButton->setEnabled(0);
        ui->quitButton->setEnabled(0);

    }
    else if (index > 0) // Because the index is -1 and 0(?) at startup.
    {
        ui->deleteButton->setEnabled(1);
        ui->startButton->setEnabled(1);
        ui->newButton->setEnabled(1);
        ui->quitButton->setEnabled(0);
        ui->nameLineEdit->setText(QString::fromStdString(users[index-1]->getName()));
        ui->levelLineEdit->setText("Level: " + QString::number(users[index-1]->getLevel()));

        currentUser = users[index-1];
    }
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

    // Checking if user name is valid size
    if (newUserName.size() > 10)
    {
        qDebug() << "Username cannot be more than 10 characters.";
        return;
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("New User");
    msgBox.setText("Create new user: " + newUserName + "?");
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, QString("OK"));
    msgBox.setButtonText(QMessageBox::No, QString("Cancel"));
    msgBox.setDefaultButton(QMessageBox::Yes);
    if(msgBox.exec() == QMessageBox::Yes){
        User * newUser = new User;
        newUser->setName(newUserName.toStdString()); // Setting name of user to the typed name on nameLineEdit
        newUser->setLevel(1); // Setting level to 1 or else newUser gets random level.
        users.push_back(newUser);
        ui->userComboBox->addItem(QString::fromStdString(newUser->getName()), int(users.size())); // Adding user to the userComboBox
        ui->userComboBox->currentIndexChanged(int(users.size()));
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
    ui->quitButton->setEnabled(1);
    ui->userComboBox->setEnabled(0);
    ui->nameLineEdit->setReadOnly(1);

    // Setting currentLevel and curentZombieSequence depending on the selected user when start was clicked
    currentLevel = levels[currentUser->getLevel()-1];
    currentZombieSequence = currentLevel->getZombieSequence();

    // Depending on user's current level, the playable grid and lawn will be different
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
    else if(currentLevel->getLevelNumber() >= 3)
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

    QRectF lawnRect(lawnLeft,lawnTop,lawnWidth,lawnHeight); // Creating a lawn rectangle with bounds left,up,width,height

    // **************************************
    // Draw lines to help out with general visualization - commented out for playing

    /*QPen my_pen = QPen(Qt::black);
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
    }*/

    // ************************************

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

    // Set up a timer which will periodically check the plant items such as sun drops
    plantItemTimer = new QTimer(this);
    connect(plantItemTimer, SIGNAL(timeout()), this, SLOT(plantItemChecker()));
    plantItemTimer->start(50);

    // Set up a timer which will periodically check projectile status and collision
    projectileTimer = new QTimer(this);
    connect(projectileTimer, SIGNAL(timeout()), this, SLOT(projectileChecker()));
    projectileTimer->start(50);

    // Set up a timer which will periodically check game end
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
    msgBox.setButtonText(QMessageBox::Yes, QString("OK"));
    msgBox.setButtonText(QMessageBox::No, QString("Cancel"));
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes)
    {
        // Deleting all current objects
        for(int i=0; i<int(existingPlants.size() && !existingPlants.empty()); i++)
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

        // Resetting counters
        zombieInterval = 0;
        lastZombieSpawnTime = 0;
        zombieCounter = 0;
        zombieTimeCounter = 0;
        sunTimeCounter = 0;
        sunPoints = 1000;

        // Resetting progress bars
        ui->progressBar_1->setValue(ui->progressBar_1->maximum());
        ui->progressBar_2->setValue(ui->progressBar_2->maximum());
        ui->progressBar_3->setValue(ui->progressBar_3->maximum());
        ui->progressBar_4->setValue(ui->progressBar_4->maximum());
        ui->progressBar_5->setValue(ui->progressBar_5->maximum());
        ui->progressBar_6->setValue(ui->progressBar_6->maximum());
        ui->progressBar_7->setValue(ui->progressBar_7->maximum());
        ui->progressBar_8->setValue(ui->progressBar_8->maximum());

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
        levelEndTimer->start(50);
        return;
    }



}

void MainWindow::on_quitButton_clicked()
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
    msgBox.setWindowTitle("Quit");
    msgBox.setText("Are you sure you want to leave this level?");
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, QString("OK"));
    msgBox.setButtonText(QMessageBox::No, QString("Cancel"));
    msgBox.setDefaultButton(QMessageBox::Yes);

    if(msgBox.exec() == QMessageBox::Yes)
    {
        // Saving - WIP
        //saveFile(users, currentUser);

        // Deleting all current objects
        for(int i=0; i<int(existingPlants.size() && !existingPlants.empty()); i++)
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
        delete gameManager;

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

        qApp->quit();
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
        levelEndTimer->start(50);
    }
}
