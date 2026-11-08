#include "brakeRow.h"


BrakeRow::BrakeRow(/* args */)
{
    parkBrakes[RIGHT] = new ParkBrake();
    parkBrakes[LEFT] = new ParkBrake();

    serviceBrakes[RIGHT] = new ServiceBrake();
    serviceBrakes[LEFT] = new ServiceBrake();

    releaseBrakes[RIGHT] = new ReleaseBrake();
    releaseBrakes[LEFT] = new ReleaseBrake();
    numberOfActive = 0;
}

void BrakeRow::setTemp(int tempRight, int tempLeft)
{
    temp[RIGHT] = tempRight;
    temp[RIGHT] = tempLeft;
    totalTemp = tempLeft + tempRight;
}
