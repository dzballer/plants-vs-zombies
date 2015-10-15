#include "level.h"

Level::Level()
{
}

Level::Level(int level_number, std::vector<int> zombie_sequence, int row_number, int start_time, int interval, double interval_decrement)
{
    levelNumber = level_number;
    zombieSequence = zombie_sequence;
    rows = row_number;
    startTime = start_time;
    this->interval = interval;
    intervalDecrement = interval_decrement;
}
int Level::getLevelNumber() const
{
    return levelNumber;
}

void Level::setLevelNumber(int value)
{
    levelNumber = value;
}
int Level::getNUMBER_OF_ZOMBIES() const
{
    return NUMBER_OF_ZOMBIES;
}

void Level::setNUMBER_OF_ZOMBIES(int value)
{
    NUMBER_OF_ZOMBIES = value;
}
std::vector<int> Level::getZombieSequence() const
{
    return zombieSequence;
}

void Level::setZombieSequence(const std::vector<int> &value)
{
    zombieSequence = value;
}
int Level::getStartTime() const
{
    return startTime;
}

void Level::setStartTime(int value)
{
    startTime = value;
}
int Level::getInterval() const
{
    return interval;
}

void Level::setInterval(int value)
{
    interval = value;
}
double Level::getIntervalDecrement() const
{
    return intervalDecrement;
}

void Level::setIntervalDecrement(double value)
{
    intervalDecrement = value;
}
int Level::getRows() const
{
    return rows;
}

void Level::setRows(int value)
{
    rows = value;
}






