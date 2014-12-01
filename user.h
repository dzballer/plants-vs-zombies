#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

using std::string;

class User
{
private:
    string name;
    int level;
    double timestamp;
public:
    User();
    User(int time_stamp, string name, double level);
    void setName(string name);
    void setLevel(int level);
    void setTimeStamp(double time_stamp);
    string getName();
    int getLevel();
    double getTimeStamp();
};

#endif // USER_H
