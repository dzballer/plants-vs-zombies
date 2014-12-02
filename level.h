#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

class Level
{
public:
    Level();
    Level(int level_number, std::vector<int> zombie_sequence, int row_number,
          int start_time, int interval, double interval_decrement);
    //void setLevelNumber(int level_number);
    //void setRows(int row_number);
    //void setStartTime(int start_time);
    //void setInterval(int interval);
    //void setIntervalDecrement(double interval_decrement);
    int getLevelNumber() const;
    void setLevelNumber(int value);

    int getNUMBER_OF_ZOMBIES() const;
    void setNUMBER_OF_ZOMBIES(int value);

    std::vector<int> getZombieSequence() const;
    void setZombieSequence(const std::vector<int> &value);

    int getStartTime() const;
    void setStartTime(int value);

    int getInterval() const;
    void setInterval(int value);

    double getIntervalDecrement() const;
    void setIntervalDecrement(double value);

    int getRows() const;
    void setRows(int value);

private:
    int levelNumber;
    int NUMBER_OF_ZOMBIES;
    std::vector<int> zombieSequence;
    int rows;
    int startTime;
    int interval;
    double intervalDecrement;
};

#endif // LEVEL_H
