#include "gamemanager.h"
#include <QDateTime>

void GameManager::readPlayersFile(QString file_name)
{


    /*string playersFileName = "pvz_players.csv";
    string levelsFileName = "pvz_levels.csv";

    QFile playersFile;*/

    mFile.setFileName(file_name);
    qDebug() << file_name;

    //QFileInfo file(mFile); // to get path
    //mPath = file.path();

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
        QStringList fileList = currentLine.split(":");

        if (fileList.count() == 3) // Checking if User file or Players file
        {
            //QRegExp re("\\d*"); // a digit (\d), zero or more times (*)

            //if (fileList.at(1).contains(QRegExp( "[-`~!@#$%}^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")) || /*!fileList.at(1).contains("[^a-zA-Z\\d\\s]") ||*/ fileList.at(1).size() > 10)
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
            /*{
                qDebug() << fileList.at(1) << "'s' data could not be read. File will be discarded.\n";
                mFile.close();
                return;
            }*.

            QRegExp rx("\\w{0,9}");
                QRegExpValidator validator (rx,0);
             int pos=0;
             if (validator.validate(userName,pos)==QValidator::Acceptable|| validator.validate(userName,pos)==QValidator::Intermediate)
             {
                 qDebug()<<"validation complete";
                 return 1;

             }
             else {qDebug()<<"not validated";
                 return false;}*/

            //  Creating aUser on the heap and passing fileList elements to the optional constructor,
            // thereby assigning the level, name, and timestamp to the new User.
            User * aUser = new User(fileList.at(0).toInt(), fileList.at(1).toStdString(), fileList.at(2).toInt()); // * remember to delete
            userVector.push_back(aUser);
            qDebug() << fileList.at(1) << "'s user data read and parsed successfully.";
            //delete aUser;
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

void GameManager::readLevelsFile(QString file_name)
{
    /*string playersFileName = "pvz_players.csv";
    string levelsFileName = "pvz_levels.csv";

    QFile playersFile;*/

    mFile.setFileName(file_name);
    qDebug() << file_name;

    //QFileInfo file(mFile); // to get path
    //mPath = file.path();

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

        /*QRegExp rx("\\w{0,9}");
            QRegExpValidator validator (rx,0);
         int pos=0;
         if (validator.validate(userName,pos)==QValidator::Acceptable|| validator.validate(userName,pos)==QValidator::Intermediate)
         {
             qDebug()<<"validation complete";
             return 1;

         }
         else {qDebug()<<"not validated";
             return false;}*/

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
            qDebug() << "Level " << fileList.at(0) << " level data read and parsed successfully.";
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
            qDebug() << fileList.at(1) << "'s plant data read and parsed successfully.";
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

void GameManager::saveFile(vector<User *> user_vector, User * current_user)
{
    QFile save_file("pvz_players.csv");

    current_user->setTimeStamp(QDateTime::currentDateTime().toTime_t()); // Retrieving and updating timestamp to the current user that is passed to the function

    if (save_file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {
     QTextStream out(&save_file);
     for (int i = 0; i<int(user_vector.size()); i++)
     {
         out << user_vector[i]->getTimeStamp() << ":" << user_vector[i]->getName().c_str() << ":" << user_vector[i]->getLevel() << "\n";
     }
     // Save data. E.g.: out << timestamp << ":" << name << ":" << lvl << "\n"
     save_file.close();
    }
    else
    {
        qDebug() << "Error: Unable to save file.\n";
    }
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


/*
QImage GameManager::retImage(QString image_path)
{
    QImage * pImage;
    QString imagePath = mPath;
    mCurrentImageName = mImageList[mIndex];

    pImage = new QImage();
    if(!(pImage->load(imagePath + "//" + mCurrentImageName)))
    {
            mCurrentImage = NULL;
            mStatusMsg = "Error: Image file could not be loaded.";
            qDebug() << "Error: Image file could not be loaded.";
            return;
    }
    return pImage;
}
*/

/*void GameManager::displayImage(QImage * image)
{
    QImage * pImage;

    pImage = image;
    if(!pImage)
    {
        qDebug() << "Error: Unable to display image.";
        return;
    }
    else
    {
        QPixmap pixma;
        pixma = QPixmap::fromImage(*pImage);
        //pixma = pixma.scaledToWidth(); // scaling pixma
        delete pImage;
    }
}*/

/*void GameManager::addUser()
{

}
*/
