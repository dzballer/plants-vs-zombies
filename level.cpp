#include "level.h"

Level::Level()
{
}

Level::Level(int level_number, std::vector<int> zombie_sequence, int row_number, int start_time, int interval, int interval_decrement)
{
    levelNumber = level_number;
    zombieSequence = zombie_sequence;
    rows = row_number;
    startTime = start_time;
    this->interval = interval;
    intervalDecrement = interval_decrement;
}
