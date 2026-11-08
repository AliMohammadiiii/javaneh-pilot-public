#include <Can.h>

HardwareSerial ESP32Serial(PA3, PA2);
HardwareSerial mySerial(PB11, PB10);
CAN_msg_t CAN_TX_msg;

void setup() {
  ESP32Serial.begin(115200);
  mySerial.begin(115200);
  cansetup();
}

void loop() {
  if (ESP32Serial.available() > 0) {
    send(ESP32Serial.read());
  }
}
void cansetup(){
  CANInit(CAN_500KBPS, 0);
  int side = analogRead(PA0);
  if(side > 1000){ // 1023 P, else N 
    CAN_TX_msg.id = 0x14;
  }
  else
  {
    CAN_TX_msg.id = 0x15;
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
void send(int speed){
    CAN_TX_msg.data[0] = speed;
    CAN_TX_msg.data[1] = 0;
    CAN_TX_msg.data[2] = 0;
    CAN_TX_msg.data[3] = 0;
    CAN_TX_msg.data[4] = 0;
    CAN_TX_msg.data[5] = 0;
    CAN_TX_msg.data[6] = 0;
    CAN_TX_msg.data[7] = 0;
    CANSend(&CAN_TX_msg);
}