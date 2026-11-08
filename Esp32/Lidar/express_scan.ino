#include "rpLidar.h"
#include "rpLidarTypes.h"

rpLidar lidar(&Serial1,1000000,&Serial);



void setup() {
  Serial.begin(115200);
  lidar.resetDevice(); //reset the device to be sure that the status is good
  lidar.setAngleOfInterest(0,360); //Set the field of view that is saved to Data
  lidar.start(express); //start the express scan of the lidar
}
void loop()
{
  lidar.readMeasurePoints();
}