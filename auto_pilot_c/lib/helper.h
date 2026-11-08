#ifndef HELPER_H
#define HELPER_H

#include <iostream>
// #include "kuksa.grpc.pb.h"
// #include "javaneh.grpc.pb.h"

#ifndef KClient_H
#include "kclient.h"
#endif


// using namespace std;
// using namespace Tiro;


class Helper 
{	
	private:
		std::string points_paths[13] = {
					"Vehicle.Hardware.RemoteController.Speed",  // 0
					"Vehicle.Hardware.RemoteController.SpeedDir",  // 1
					"Vehicle.Hardware.RemoteController.StAngle", // 2
					"Vehicle.Hardware.RemoteController.StAngleDir", // 3
					"Vehicle.Hardware.RemoteController.isSafeMode", // 4
					"Vehicle.Hardware.RemoteController.isTurningPlace", // 5
					"Vehicle.Hardware.RemoteController.mission", // 6
					"Vehicle.Hardware.RemoteController.mode", // 7
					"Vehicle.Hardware.Surround.GPS.YN.lat",  // 8
					"Vehicle.Hardware.Surround.GPS.YN.lon", // 9
					"Vehicle.Hardware.Surround.CompassAngle", //  10
					"Vehicle.Hardware.FrontSide", // 11 , 0 XP , 1 XN , 2 FrontSide is not set
					"Vehicle.Software.FrontSide" // 12 , 0 XP , 1 XN , 2 FrontSide is not set
		};
	public:
		Helper();
		// int getPoint(KukClient* vehicle, Point* point);
		int vehicleSide(int hardware_front_side, int software_front_side, int auto_pilot_mode);
	uint64_t timeSinceEpochMillisec();
};
#endif
