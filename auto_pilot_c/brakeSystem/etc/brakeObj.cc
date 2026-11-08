#include "brakeObj.h"
Brake::Brake(/* args */)
{
}

ParkBrake::ParkBrake(/* args */)
{
    value = 1;
    isUsed = false;
}


ServiceBrake::ServiceBrake(/* args */)
{
    value = 0;
    isUsed = false;
}


ReleaseBrake::ReleaseBrake(/* args */)
{
    value = 0;
    isUsed = false;
}

void Brake::setPath(string _path)
{
    path = _path;
}


bool ParkBrake::active() 
{
    value = 0;
    isUsed = true;
    return false;
}

bool ServiceBrake::active() 
{
    value = 1;
    isUsed = true;
    return true;
}

bool ReleaseBrake::active()
{
    value = 1;
    isUsed = true;
    return false;
}

bool ParkBrake::deactive()
{
    value = 1;
    isUsed = false;
    return false;
}

bool ServiceBrake::deactive()
{
    value = 0;
    isUsed = false;
    return false;
}

bool ReleaseBrake::deactive()
{
    value = 0;
    isUsed = false;
    return false;
}