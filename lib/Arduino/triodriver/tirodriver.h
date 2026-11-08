#ifndef TIRODRIVER_H
#define TIRODRIVER_H
#include <Arduino.h>
struct Driver{
  int RPWM;
  int LPWM;
  int REN;
  int LEN;
  int PWMNow=0;
};
Driver init_driver(int RPWM, int LPWM, int REN, int LEN);
void controlDriver(Driver driver,int direction);
#endif
