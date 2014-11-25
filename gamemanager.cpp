#include "gamemanager.h"

void GameManager::readFile(QString file_name)
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
            //  Creating aUser on the heap and passing fileList elements to the optional constructor,
            // thereby assigning the level, name, and timestamp to the new User.
            User * aUser = new User(fileList.at(0).toInt(), fileList.at(1).toStdString(), fileList.at(2).toInt()); // * remember to delete
            userVector.push_back(aUser);
            qDebug() << fileList.at(1) << "'s user data read and parsed successfully.";
        }
        else if (fileList.count() == 6)
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

            delete aLevel;
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
