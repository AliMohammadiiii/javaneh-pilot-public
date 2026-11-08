
#include <Can.h>



#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_u-blox_GNSS
SFE_UBLOX_GPS myGPS;

// #include <SoftwareSerial.h>
// SoftwareSerial mySerial(PA3, PA2); // RX, TX. Pin 10 on Uno goes to TX pin on GPS module.
HardwareSerial mySerial(PA3, PA2);
HardwareSerial debug_serial(PB11, PB10);
long lastTime = 0; //Simple local timer. Limits amount of I2C traffic to Ublox module.
CAN_msg_t CAN_TX_msg;
int32_t _latitude;
int32_t _longitude;



void cansetup(){
  bool ret = CANInit(CAN_125KBPS, 0);
  
  int side = analogRead(PA0);
  if(side > 1000){ // 1023 P, else N 
    CAN_TX_msg.id = 0x110;
  }
  else
  {
    CAN_TX_msg.id = 0x111;
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
void setup()
{

  
  debug_serial.begin(115200);
  debug_serial.println("STARTTTTTTTTTTTTTT");
  
  while (!debug_serial); //Wait for user to open terminal

  cansetup();
  
  debug_serial.println("SparkFun Ublox Example");
  return;
  //Assume that the U-Blox GPS is running at 9600 baud (the default) or at 38400 baud.
  //Loop until we're in sync and then ensure it's at 38400 baud.
  do {
    Serial.println("GPS: trying 38400 baud");
    mySerial.begin(38400);
    if (myGPS.begin(mySerial) == true) break;

    delay(100);
    debug_serial.println("GPS: trying 9600 baud");
    mySerial.begin(9600);
    if (myGPS.begin(mySerial) == true) {
        debug_serial.println("GPS: connected at 9600 baud, switching to 38400");
        myGPS.setSerialRate(38400);
        delay(100);
    } else {
        //myGPS.factoryReset();
        delay(2000); //Wait a bit before trying again to limit the Serial output
    }
  } while(1);
  debug_serial.println("GPS serial connected");
  myGPS.enableDebugging(debug_serial);
  myGPS.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
  myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGPS.saveConfiguration(); //Save the current settings to flash and BBR
}
void send(int32_t latitude, int32_t longitude){
  if(_latitude != latitude || _longitude != longitude){
    CAN_TX_msg.data[0] = (latitude >> 24) & 0xff;
    CAN_TX_msg.data[1] = (latitude >> 16) & 0xff;
    CAN_TX_msg.data[2] = (latitude >> 8) & 0xff;
    CAN_TX_msg.data[3] = latitude & 0xff;
    CAN_TX_msg.data[4] = (longitude >> 24) & 0xff;
    CAN_TX_msg.data[5] = (longitude >> 16) & 0xff;
    CAN_TX_msg.data[6] = (longitude >> 8) & 0xff;
    CAN_TX_msg.data[7] = longitude & 0xff;
    CANSend(&CAN_TX_msg);
    debug_serial.println("SEND CANNNNNNNN");
    // _latitude = latitude;
    // _longitude = longitude;
  }
}
void loop()
{
  int32_t latitude = 1;
  int32_t longitude = 2;
  debug_serial.println("LOG 1 ");
  send(latitude, longitude);
  debug_serial.println("LOG 2 ");
  delay(100);

  return;
  int side = analogRead(PA0);
  debug_serial.print("SIDE : ");
  debug_serial.println(side);
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  if (millis() - lastTime > 100)
  {
    lastTime = millis(); //Update the timer
    int32_t latitude = myGPS.getLatitude();
    int32_t longitude = myGPS.getLongitude();
    send(latitude, longitude);
    
  }
}
