#include "user.h"

User::User()
{
}

User::User(int time_stamp, string name, double level_number)
{
    setLevel(level_number);
    setName(name);
    setTimeStamp(time_stamp);
}

void User::setName(string name)
{
    this->name = name;
}

void User::setLevel(int level_number)
{
    this->level = level_number;
}

void User::setTimeStamp(double time_stamp)
{
    timestamp = time_stamp;
}

string User::getName()
{
    return name;
}

int User::getLevel()
{
    return level;
}

double User::getTimeStamp()
{
    return timestamp;
}

