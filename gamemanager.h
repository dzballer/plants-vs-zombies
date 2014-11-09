#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "user.h"
#include "level.h"
#include <vector>

class GameManager
{
private:
    std::vector<User> userVector;
    std::vector<Level> levelVector;

public:
    void loadFile();
    GameManager();
};

#endif // GAMEMANAGER_H
