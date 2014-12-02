#include "gamemanager.h"
#include <QDateTime>
#include <QApplication>
#include <QTimer>
void GameManager::readPlayersFile(QString file_name)
{
    mFile.setFileName(file_name);
    qDebug() << file_name;

    if (!mFile.open((QIODevice::ReadOnly | QIODevice::Text))) // error checking
    {
       //qDebug() << "Unable to open file for reading: An error has occurred.";
       return;
    }

    QTextStream in(&mFile);

    while (!in.atEnd())
    {
        int i = 1; // Using as a line counter for debugging
        QString currentLine = in.readLine();
        QStringList fileList = currentLine.split(":");

        if (fileList.count() == 3) // Checking if appropriate amount of elements are in csv
        {
            // Checking if alphanumeric
            for (int j = 0; j < fileList.at(1).size(); j++)
            {
                if (!fileList.at(1).at(j).isLetterOrNumber())// && !fileList.at(1).at(j).isSpace())
                {
                    qDebug() << fileList.at(1) << "'s' player-name is not alpha-numeric. File will be discarded. Program will start with no users.";
                    mFile.flush();
                    mFile.close();
                    userVector.clear(); // Clearing previously read data
                    return;
                }
            }

            // Checking to see if player name is greater than 10 characters
            for (int j = 0; j < fileList.at(1).size(); j++)
            {
                if (fileList.at(1).size() > 10)// && !fileList.at(1).at(j).isSpace())
                {
                    qDebug() << fileList.at(1) << "'s' player-name is too long (>10 characters). Program will start with no users.";
                    mFile.flush();
                    mFile.close();
                    userVector.clear(); // Clearing previously read data
                    return;
                }
            }

            // Checking if level contains any characters other than 0-9
            for (int j = 0; j < fileList.at(2).size(); j++)
            {
                if (!fileList.at(2).at(j).isNumber())
                {
                    //qDebug() << "Level data contains characters other than 0-9. File will be discarded.";
                    mFile.flush();
                    mFile.close();
                    return;
                }
            }

            // Checking if Level is <0 or >100
            if (fileList.at(2).toInt()<0 || fileList.at(2).toInt()>100)
            {
                qDebug() << "Error: Level number must be between 0 and 100.\n";
                mFile.flush();
                mFile.close();
                return;
            }

            //  Creating aUser on the heap and passing fileList elements to the optional constructor,
            // thereby assigning the level, name, and timestamp to the new User.
            User * aUser = new User(fileList.at(0).toInt(), fileList.at(1).toStdString(), fileList.at(2).toInt()); // * remember to delete
            userVector.push_back(aUser);
            //qDebug() << fileList.at(1) << "'s user data read and parsed successfully.";
            //delete aUser;
        }
        else
        {
            qDebug() << "Unable to read/parse file on line" << i << ": Invalid Player Data: Program will start with no users.";
            mFile.flush();
            mFile.close();
            return;
        }

        i++; // Increasing line count
    }

    mFile.flush();
    mFile.close(); // Flushes then closes file
}

void GameManager::readLevelsFile(QString file_name)
{
    qDebug() << file_name;

    mFile.setFileName(file_name);

    if (!mFile.open((QIODevice::ReadOnly | QIODevice::Text))) // error checking
    {
       qDebug() << "Could not open " << file_name;
       qDebug() << "Program will now terminate.";
       qApp->quit();
       QTimer * quitTimer = new QTimer;
       quitTimer->singleShot(250,qApp,SLOT(quit())); // Quits program
       delete quitTimer;
       return;
    }

    QTextStream in(&mFile);

    while (!in.atEnd())
    {
        int i = 1; // Using as a line counter for debugging
        QString currentLine = in.readLine();
        QStringList fileList = currentLine.split(":");

        QRegExp re("\\d*"); // a digit (\d), zero or more times (*)
        if (!re.exactMatch(fileList.at(0)))
        {
            qDebug() << "Error: Level number can only contain numerical digits (0-9).\n";
            mFile.close();
            return;
        }

        if (fileList.at(0).toInt()<0 || fileList.at(0).toInt()>10)
        {
            qDebug() << "Error: Level number must be between 0 and 10.\n";
            mFile.close();
            return;
        }

        if (fileList.count() == 6)
        {

            //  Creating aLevel on the heap and passing fileList elements to the optional constructor,
            // thereby assigning the level, zombie sequence, rows, startTime, interval, intervalDecrement to the new User.
            QStringList sequenceList = fileList.at(1).split(",");
            std::vector<int> intVector;
            for (int i = 0; i < sequenceList.count(); i++)
            {
                intVector.push_back(sequenceList.at(i).toInt());
            }
            Level * aLevel = new Level(fileList.at(0).toInt(), intVector, fileList.at(2).toInt(),
                                       fileList.at(3).toInt(), fileList.at(4).toInt(), fileList.at(5).toDouble());
            levelVector.push_back(aLevel); // *remember to delete
            //qDebug() << "Level " << fileList.at(0) << " level data read and parsed successfully.";
            //delete aLevel;
        }
        else
        {
            qDebug() << "Unable to read/parse file on line" << i << ": File has errors.";
            return;
        }

        i++; // Increasing line count
    }

    mFile.flush();
    mFile.close(); // Flushes then closes file
}

void GameManager::readPlantsFile(QString file_name)
{
    mFile.setFileName(file_name);
    qDebug() << file_name;

    if (!mFile.open((QIODevice::ReadOnly | QIODevice::Text))) // error checking
    {
       qDebug() << "Unable to open file for reading: An error has occurred.";
       return;
    }

    QTextStream in(&mFile);

    while (!in.atEnd())
    {
        int i = 1; // Using as a line counter for debugging
        QString currentLine = in.readLine();
        QStringList fileList = currentLine.split(" ");

        if (fileList.count() == 13) // Checking if User file or Players file
        {
            // Checking if alphanumeric
            for (int i = 0; i < fileList.count(); i++)
            {
                for (int j = 0; j < fileList.at(1).size(); j++)
                {
                    if (!fileList.at(1).at(j).isLetterOrNumber() && !fileList.at(1).at(j).isSpace())
                    {
                        qDebug() << fileList.at(1) << "'s' data could not be read. File will be discarded.\n";
                        mFile.close();
                        return;
                    }
                }
            }

            // Creating aPlant and assigning all members from plants data file
            Plant * aPlant = new Plant;
            aPlant->setIndex(fileList.at(0).toInt());
            aPlant->setName(fileList.at(1).toStdString());
            aPlant->setCost(fileList.at(2).toInt());
            aPlant->setLife(fileList.at(3).toInt());
            aPlant->setRange(fileList.at(4).toInt());
            aPlant->setDamage(fileList.at(5).toInt());
            aPlant->setFireRate(fileList.at(6).toDouble());
            aPlant->setSplash(fileList.at(7).toInt());
            aPlant->setSlow(fileList.at(8).toInt());
            aPlant->setBomb(fileList.at(9).toInt());
            aPlant->setSeeding(fileList.at(10).toInt());
            aPlant->setSun(fileList.at(11).toInt());
            aPlant->setNeed(fileList.at(12).toInt());
            plantVector.push_back(aPlant);
            //qDebug() << fileList.at(1) << "'s plant data read and parsed successfully.";
            //delete aPlant;
        }
        else
        {
            qDebug() << "Unable to read/parse file on line" << i << ": File has errors.";
            return;
        }
        i++; // Increasing line count
    }

    mFile.flush();
    mFile.close(); // Flushes then closes file
}

void GameManager::readZombiesFile(QString file_name)
{
    mFile.setFileName(file_name);
    qDebug() << file_name;

    if (!mFile.open((QIODevice::ReadOnly | QIODevice::Text))) // error checking
    {
       qDebug() << "Unable to open file for reading: An error has occurred.";
       return;
    }

    QTextStream in(&mFile);

    int i = 1; // Using as a line counter for debugging

    while (!in.atEnd())
    {
        QString currentLine = in.readLine();
        QStringList fileList = currentLine.split(" ");

        if (fileList.count() == 7) // Checking #elements
        {
            // Checking if alphanumeric
            for (int i = 0; i < fileList.count(); i++)
            {
                for (int j = 0; j < fileList.at(1).size(); j++)
                {
                    if (!fileList.at(1).at(j).isLetterOrNumber() && !fileList.at(1).at(j).isSpace())
                    {
                        qDebug() << fileList.at(1) << "'s' data could not be read. File will be discarded.\n";
                        mFile.close();
                        return;
                    }
                }
            }

            // Creating aZombie and assigning all members from zombie data file
            Zombie * aZombie = new Zombie;
            aZombie->setIndex(fileList.at(0).toInt());
            aZombie->setName(fileList.at(1).toStdString());
            aZombie->setArmor(fileList.at(2).toInt());
            aZombie->setLife(fileList.at(3).toInt());
            aZombie->setAttack(fileList.at(4).toInt());
            aZombie->setAttackRate(fileList.at(5).toDouble());
            aZombie->setSpeed(fileList.at(6).toDouble());

            zombieVector.push_back(aZombie);
            //qDebug() << fileList.at(1) << "'s zombie data read and parsed successfully.";
            //delete aPlant;
        }
        else
        {
            qDebug() << "Unable to read/parse file on line" << i << ": Not enough data elements.";
            return;
        }
        i++; // Increasing line count
    }

    mFile.flush();
    mFile.close(); // Flushes then closes file
}

vector<User *> GameManager::getUserVector()
{
    return userVector;
}

vector<Level *> GameManager::getLevelVector()
{
    return levelVector;
}

vector<Plant *> GameManager::getPlantVector()
{
    return plantVector;
}

vector<Zombie *> GameManager::getZombieVector()
{
    return zombieVector;
}
