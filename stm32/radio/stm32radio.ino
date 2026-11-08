#include <Wire.h>
#define _VARIANT_ARDUINO_STM32_ 
#include <IBusBM.h>
#include <Can.h>
#define _i2cAddress 0x60  //Address of the CMPS12 compass on i2C
#define TWO_BYTES 2
HardwareSerial mySerial(PB11, PB10);
int valueRadio[14];
int ForceBreak=0;
IBusBM ibusRc;
HardwareSerial ibusRcSerial(PA3, PA2);
CAN_msg_t canSendMsg;
long read_time_delay = 0; //Simple local timer. Limits amount of I2C traffic to Ublox module.


void cansetup(){
  CANInit(CAN_500KBPS, 0);
  
  canSendMsg.data[0] = 0x00;
  canSendMsg.data[1] = 0x00;
  canSendMsg.data[2] = 0x00;
  canSendMsg.data[3] = 0x00;
  canSendMsg.data[4] = 0x00;
  canSendMsg.data[5] = 0x00;
  canSendMsg.data[6] = 0x00;
  canSendMsg.data[7] = 0x00;
  canSendMsg.len = 8;
  
  
  canSendMsg.format = STANDARD_FORMAT;
}

void setup() 
{
  ibusRc.begin(ibusRcSerial);
  cansetup();
  Wire.begin();
  mySerial.begin(115200);
  read_time_delay = millis();
}

int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){
  // we need to call loop function manually on stm32 arch
  ibusRc.loop();
  uint16_t ch = ibusRc.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
  for (byte i = 0; i<14; i++){
    valueRadio[i] = 0;
  } 
}

void loop() 
{
  radio_read();
}

void radio_can_msg( int mission,int speed,int angle,int notused,byte carmode){
  byte signs = 0b0000000;
    canSendMsg.id  = 17;
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

    CANSend(&canSendMsg);
}



void radio_read()
{
  byte carmode = 0b00000000;
  if (millis()-read_time_delay>100)
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