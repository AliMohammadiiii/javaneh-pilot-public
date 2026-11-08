#include <IBusBM.h>
#include <mcp2515.h>
IBusBM ibusRc;
MCP2515 mcp2515(53);

HardwareSerial& ibusRcSerial = Serial;
HardwareSerial& debugSerial = Serial;
int valueRadio[14];
long int read_time_delay;
long int can_lazer_time_delay;

struct can_frame canMsg;
struct can_frame canSendMsg;


void setup() {
  debugSerial.begin(74880);
  ibusRc.begin(ibusRcSerial);
  can_lazer_time_delay = millis();
  read_time_delay = millis();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();
}

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){
  uint16_t ch = ibusRc.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
  for (byte i = 0; i<14; i++){
    valueRadio[i] = 0;
  } 
}

// Red the channel and return a boolean value
bool redSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void loop() {
  if (millis()-read_time_delay>100)
  {
    read_time_delay = millis();
    for (byte i = 0; i<14; i++){
    int value = readChannel(i, -255, 255, 0);
    //if (valueRadio[i] != value)
    //{
      can_msg(i+1, value);
      valueRadio[i] = value;
      debugSerial.print("Ch");
      debugSerial.print(i + 1);
      debugSerial.print(": ");
      debugSerial.print(value);
      debugSerial.print(" ");
    //}
  }
  debugSerial.println();
  }
  // debugSerial.print("Ch5: ");
  // debugSerial.print(redSwitch(4, false));
  // delay(10);
}


void can_Send(can_frame canSendMsg) {
  mcp2515.sendMessage(&canSendMsg);
    // Serial.print("ID= ");
    // Serial.print(canSendMsg.can_id,HEX);
    // Serial.print("   State:");
    // Serial.println(mcp2515.sendMessage(&canSendMsg),HEX);
    // delay(20);
}
void can_msg(int ch, int value){
  if(millis()-can_lazer_time_delay>0){
    can_lazer_time_delay=millis();
    canSendMsg.can_id  = 17;
    canSendMsg.can_dlc = 8;
    canSendMsg.data[0] = ch; 
    canSendMsg.data[1] = abs(value);
    canSendMsg.data[2] = 0; 
    if(value > 0)
      canSendMsg.data[2] = 1;     
    canSendMsg.data[3] = 0;
    canSendMsg.data[4] = 0;
    canSendMsg.data[5] = 0;
    canSendMsg.data[6] = 0;
    canSendMsg.data[7] = 0;
    can_Send(canSendMsg);
  }
}