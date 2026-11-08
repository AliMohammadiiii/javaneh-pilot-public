#include <tirodriver.h>
#include <mcp2515.h>
#include <Wire.h>
#include <IBusBM.h>
IBusBM ibusRc;
#define _i2cAddress 0x60  //Address of the CMPS12 compass on i2C
// #define BEARING_Register 2 
#define TWO_BYTES 2
HardwareSerial& ibusRcSerial = Serial1;
int valueRadio[14];
struct can_frame canMsg;
struct can_frame canSendMsg;
MCP2515 mcp2515(53);
const int driver1_RPWM  = 12;
const int driver1_LPWM  = 13;
const int driver1_REN   = 48; 
const int driver1_LEN   = 49;
const int driver0_RPWM  = 11; 
const int driver0_LPWM  = 10; 
const int driver0_REN   = 40;
const int driver0_LEN   = 41;
long int can_lazer_time_delay;
long int read_time_delay;
// long int bearing_time_delay;
struct Driver driver[2];
int ForceBreak=0;
int counter = 0;
// int _bearing;
int nReceived;
byte _fine;
byte _byteHigh;
byte _byteLow;

void init_motor_driver()
{
  driver[0]=init_driver(driver0_RPWM,driver0_LPWM,driver0_REN,driver0_LEN);
  driver[1]=init_driver(driver1_RPWM,driver1_LPWM,driver1_REN,driver1_LEN);
  driver[0].PWMNow = 0;
  driver[1].PWMNow = 0;
  controlDriver(driver[0],0);
  controlDriver(driver[1],1);
}

void init_can()
{
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS);
  mcp2515.setNormalMode();
}

void setup() 
{
  ibusRc.begin(ibusRcSerial);
  init_motor_driver();
  init_can();
  Wire.begin();
  Serial.begin(19200);
  read_time_delay = millis();
}

int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){
  uint16_t ch = ibusRc.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
  for (byte i = 0; i<14; i++){
    valueRadio[i] = 0;
  } 
}

void loop() 
{
  can_Read();
  radio_read();
  // getBearing();
}

// void getBearing()
// {
//   if (millis()-bearing_time_delay>300)
//   {
//     bearing_time_delay = millis();

//     // Begin communication with CMPS12
//     Wire.beginTransmission(_i2cAddress);

//     // Tell register you want some data
//     Wire.write(BEARING_Register);

//     // End the transmission
//     int nackCatcher = Wire.endTransmission();

//     // Return if we have a connection problem 
//     if(nackCatcher != 0){return 0;}
    
//     // Request 2 bytes from CMPS12
//     nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

//     // Something has gone wrong
//     if (nReceived != TWO_BYTES) return 0;

//     // Read the values
//     _byteHigh = Wire.read(); 
//     _byteLow = Wire.read();

//     // Calculate full bearing
//     _bearing = ((_byteHigh<<8) + _byteLow) / 10;

//     bearing_can_msg(_bearing);
//     }
// }

void can_Read(){
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if(canMsg.can_id==81 || canMsg.can_id==87 ){//80rast 87 chap
      run(canMsg);
    }

  }
}

void can_Send(can_frame canSendMsg) {
  mcp2515.sendMessage(&canSendMsg);
}

// void bearing_can_msg(int angle){
//     canSendMsg.can_id  = 7;
//     canSendMsg.can_dlc = 8;
//     if (angle >= 256)
//       canSendMsg.data[0] = 1;
//     else
//       canSendMsg.data[0] = 0;
//     if (angle >= 256)
//       canSendMsg.data[1] = angle -256;
//     else
//       canSendMsg.data[1] = angle;    
//     canSendMsg.data[2] = 0;
//     canSendMsg.data[3] = 0;
//     canSendMsg.data[4] = 0;
//     canSendMsg.data[5] = 0; 
//     canSendMsg.data[6] = 0;
//     canSendMsg.data[7] = 0;
//     can_Send(canSendMsg);
// }
void run(can_frame canMsg)
{
  if(ForceBreak==1)
  {
    for(int i=0;i<2;i++)
    {    
      driver[i].PWMNow=0;
      controlDriver(driver[i],0);
    }
  }
  else
  {
    if(canMsg.can_id==81)
    {
    driver[0].PWMNow=canMsg.data[0];
    controlDriver(driver[0],canMsg.data[1]);
    }
    else{    
      driver[1].PWMNow=canMsg.data[0];
      controlDriver(driver[1],canMsg.data[1]);
    }
  }
}
void radio_can_msg( int mission,int speed,int angle,int notused,byte carmode){
  byte signs = 0b0000000;
    canSendMsg.can_id  = 17;
    canSendMsg.can_dlc = 8;
    if(speed > 0){
      canSendMsg.data[0] = abs(speed);
      canSendMsg.data[6] = 0;
    }
    else{
      canSendMsg.data[0] = 0;
      canSendMsg.data[6] = abs(speed);
    }
    canSendMsg.data[1] = abs(mission);
    if(mission < 0)
      bitSet(signs, 1);  //canSendMsg.data[3] = 1;
    canSendMsg.data[2] = abs(angle);
    if(angle < 0)
      bitSet(signs, 2); 
    canSendMsg.data[3] = abs(notused);
    if(notused < 0)
      bitSet(signs, 3);
      canSendMsg.data[4] = signs; 
      canSendMsg.data[5] = carmode;
    // Serial.print("signs: ");
    // Serial.print(signs, BIN);
    // Serial.print(" Can:{ ");
    // for(int i=0;i<8;i++){
    //   Serial.print(canSendMsg.data[i]); 
    //   Serial.print(" , ");
    // }
    // Serial.print(" }   Car Mode:  ");
    // Serial.println(carmode, BIN); 
    can_Send(canSendMsg);
}

// bool redSwitch(byte channelInput, bool defaultValue){
//   int intDefaultValue = (defaultValue)? 100: 0;
//   int ch = readChannel(channelInput, 0, 100, intDefaultValue);
//   return (ch > 50);
// }

void radio_read()
{
  byte carmode = 0b00000000;
  if (millis()-read_time_delay>200)
  {
    int radioData[9];
    read_time_delay = millis();
  radioData[0] = readChannel(0, -255, 255, 0);
  radioData[1] = readChannel(1, -255, 255, 0);
  radioData[2] = readChannel(2, -255, 255, 0);
  radioData[3] = readChannel(3, -255, 255, 0);
  radioData[4] = readChannel(4, -1, 1, 0);
  radioData[5] = readChannel(6, -1, 1, 0);
  radioData[6] = readChannel(7, -1, 1, 0);
  radioData[7] = readChannel(8, -1, 1, 0);
  radioData[8] = readChannel(9, -1, 1, 0);
  if(radioData[4]==-1){
    bitSet(carmode, 0);
  }
  if(radioData[5]==-1){
    bitSet(carmode, 1);
  }
  if(radioData[6]==-1){
    bitSet(carmode, 2);
  }
  if(radioData[7]==0){
    bitSet(carmode, 3);
  }
  else if(radioData[7]==-1){
    bitSet(carmode, 4);
  }
    if(radioData[8]==0){
    bitSet(carmode, 5);
  }
  else if(radioData[8]==-1){
    bitSet(carmode, 6);
  }
  radio_can_msg(radioData[0], radioData[1], radioData[2], radioData[3],carmode);
  }
}