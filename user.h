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
    User(int level, string name, double time_stamp);
    void setName(string name);
    void setLevel(int level);
    void setTimeStamp(double time_stamp);
};

#endif // USER_H
