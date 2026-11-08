#include "tirodriver.h"
Driver init_driver(int RPWM, int LPWM, int REN, int LEN){
	Driver driver;
    driver.RPWM=RPWM;
    driver.LPWM=LPWM;
    driver.REN=REN;
    driver.LEN=LEN;
    pinMode(driver.RPWM, OUTPUT);
    pinMode(driver.LPWM, OUTPUT);
    pinMode(driver.REN, OUTPUT);
    pinMode(driver.LEN, OUTPUT);
    digitalWrite(driver.REN,HIGH);
    digitalWrite(driver.LEN,HIGH);  
	return driver;
}
void controlDriver(Driver driver,int direction){
    if(direction==0){
    analogWrite(driver.RPWM,driver.PWMNow);
    analogWrite(driver.LPWM,0);
    }
    else{
      analogWrite(driver.RPWM,0);
      analogWrite(driver.LPWM,driver.PWMNow);
    }
}