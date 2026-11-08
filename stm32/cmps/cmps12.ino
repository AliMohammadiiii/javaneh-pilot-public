/*****************************************
*     CMPS12 I2C example for Arduino     *
*        By James Henderson, 2014        * 
*****************************************/

#include <Wire.h>
#include <Can.h>
#define CMPS12_ADDRESS 0x60  // Address of CMPS12 shifted right one bit for arduino wire library
// #define ANGLE_8  5        // Register to read 8bit angle from
#define ROLL_ADDRESS  5       // Register to read 8bit roll from
#define PITCH_ADDRESS  4      // Register to read 8bit roll from
HardwareSerial mySerial(PB11, PB10);
// #define PITCH_ADDRESS 0x1C
unsigned char high_byte, low_byte; 
signed char pitch, roll;
unsigned int angle16;
// signed char angle8;
CAN_msg_t CAN_TX_msg;
signed char _pitch, _roll;



void cansetup(){
  bool ret = CANInit(CAN_125KBPS, 0);
  
  int side = analogRead(PA0);
  if(side > 1000){ // 1023 P, else N 
    CAN_TX_msg.id = 0x30;
  }
  else
  {
    CAN_TX_msg.id = 0x31;
  }
  
  CAN_TX_msg.data[0] = 0x00;
  CAN_TX_msg.data[1] = 0x00;
  CAN_TX_msg.data[2] = 0x00;
  CAN_TX_msg.data[3] = 0x00;
  CAN_TX_msg.data[4] = 0x00;
  CAN_TX_msg.data[5] = 0x00;
  CAN_TX_msg.data[6] = 0x00;
  CAN_TX_msg.data[7] = 0x00;
  CAN_TX_msg.len = 8;
  
  CAN_TX_msg.format = STANDARD_FORMAT;
}

signed char get_roll()
{
  Wire.beginTransmission(CMPS12_ADDRESS);  //starts communication with CMPS12
  Wire.write(ROLL_ADDRESS);                     //Sends the register we wish to start reading from
  Wire.endTransmission();
  Wire.requestFrom(CMPS12_ADDRESS, 1);       
  while(Wire.available() < 1);        // Wait for all bytes to come back
  signed char roll_ = Wire.read();   
  return roll_; 
}

signed char get_pitch()
{
  Wire.beginTransmission(CMPS12_ADDRESS);  //starts communication with CMPS12
  Wire.write(PITCH_ADDRESS);                     //Sends the register we wish to start reading from
  Wire.endTransmission();
  Wire.requestFrom(CMPS12_ADDRESS, 1);       
  while(Wire.available() < 1);        // Wait for all bytes to come back
  signed char pitch_ = Wire.read();   
  return pitch_;    
}
void send(signed char pitch, signed char roll){
  // if(1){
  if(_pitch != pitch || _roll != roll){

    CAN_TX_msg.data[0] = abs(pitch);
    if (pitch < 0)
      CAN_TX_msg.data[1] = 1;
    else
      CAN_TX_msg.data[1] = 0;
    CAN_TX_msg.data[2] = abs(roll);
    if (roll < 0)
      CAN_TX_msg.data[3] = 1;
    else
      CAN_TX_msg.data[3] = 0;
    CAN_TX_msg.data[4] = 0;
    CAN_TX_msg.data[5] = 0;
    CAN_TX_msg.data[6] = 0;
    CAN_TX_msg.data[7] = 0;
    CANSend(&CAN_TX_msg);
    mySerial.println("SEND CANNNNNNNN");
    _pitch = pitch;
    _roll = roll;
  }
}

void setup()
{
  delay(2000);
  mySerial.begin(9600);  // Start serial port
  while (!mySerial); //Wait for user to open terminal
  Wire.begin();
  cansetup();
}

void loop()
{
  send(get_pitch(), get_roll());
  delay(20);                           // Short delay before next loop
}
