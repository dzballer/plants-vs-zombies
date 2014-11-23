#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

class Level
{
private:

public:
    Level();
    Level(int level_number, std::vector<int> zombie_sequence, int row_number,
          int start_time, int interval, int interval_decrement);
    void setLevelNumber(int level_number);
    void setRows(int row_number);
    void setStartTime(int start_time);
    void setInterval(int interval);
    void setIntervalDecrement(double interval_decrement);

    int levelNumber;
    int NUMBER_OF_ZOMBIES;
    std::vector<int> zombieSequence;
    int rows;
    int startTime;
    int interval;
    double intervalDecrement;
};

#endif // LEVEL_H
