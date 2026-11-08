#ifndef BRAKEROW_H_
#define BRAKEROW_H_


#include "brakeObj.h"
#include <vector>
#include <iostream>
#include <string.h>
using namespace std;

#define RIGHT 0
#define LEFT 1



class BrakeRow
{
private:
public:
    int id;
    int temp[2];
    int totalTemp;
    int numberOfActive;
    ParkBrake* parkBrakes[2];
    ServiceBrake* serviceBrakes[2];
    ReleaseBrake* releaseBrakes[2];
    BrakeRow(/* args */);
    void setTemp(int tempRight, int tempLeft);
    bool operator<(BrakeRow* other) const {
        return this->totalTemp < other->totalTemp;
    }
};


















#endif