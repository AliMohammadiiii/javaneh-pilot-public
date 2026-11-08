#ifndef BRAKEOBJ_H_
#define BRAKEOBJ_H_

#include <vector>
#include <iostream>
#include <string.h>
using namespace std;

class Brake
{
protected:
public:
    bool isUsed;
    int value;
    string path;
    Brake(/* args */);
    void setPath(string _path);
    virtual bool active(){}
    virtual bool deactive(){}
};





class ParkBrake : public Brake
{
private:
public:
    ParkBrake(/* args */);
    bool active();
    bool deactive();
};



class ServiceBrake : public Brake
{
private:
public:
    ServiceBrake(/* args */);
    bool active();
    bool deactive();
};


class ReleaseBrake : public Brake
{
private:
public:
    ReleaseBrake(/* args */);
    bool active();
    bool deactive();
};




#endif