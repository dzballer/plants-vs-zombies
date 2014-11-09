#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

class Level
{
private:
    int levelNumber;
    int NUMBER_OF_ZOMBIES;
    std::vector<int> zombieSequence;
    int rows;
    int startTime;
    int interval;
    double intervalDecrement;
public:
    Level();
};

#endif // LEVEL_H
