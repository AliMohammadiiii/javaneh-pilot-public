#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <bucketManagement.h>





void init()
{
	string sensorPaths[26] = {
					// "Vehicle.Body.Load.BucketAngle.x",  // 0
					// "Vehicle.Body.Load.BucketAngle.y",  // 1
					// "Vehicle.Frame.Chassis.ChassisAngle.x",  // 2
					// "Vehicle.Frame.Chassis.ChassisAngle.y",  // 3
					"Vehicle.Energy.Hydrolic.OilPumpPressure",  // 4
					"Vehicle.Energy.Hydrolic.OilPumpTemp",  // 5
					"Vehicle.Energy.Hydrolic.OilTankLevel",  // 6
					"Vehicle.Body.Load.BucketWeight.YP.XP.0.weight",  // 7
					"Vehicle.Body.Load.BucketWeight.YP.XP.1.weight",  // 8
					"Vehicle.Body.Load.BucketWeight.YP.XP.2.weight",  // 9
					"Vehicle.Body.Load.BucketWeight.YP.XP.3.weight",  // 10
					"Vehicle.Body.Load.BucketWeight.YP.XN.0.weight",  // 11
					"Vehicle.Body.Load.BucketWeight.YP.XN.1.weight",  // 12
					"Vehicle.Body.Load.BucketWeight.YP.XN.2.weight",  // 13
					"Vehicle.Body.Load.BucketWeight.YP.XN.3.weight",  // 14
					"Vehicle.Body.Load.BucketWeight.YN.XP.0.weight",  // 15
					"Vehicle.Body.Load.BucketWeight.YN.XP.1.weight",  // 16	
					"Vehicle.Body.Load.BucketWeight.YN.XP.2.weight",  // 17
					"Vehicle.Body.Load.BucketWeight.YN.XP.3.weight",  // 18
					"Vehicle.Body.Load.BucketWeight.YN.XN.0.weight",  // 19
					"Vehicle.Body.Load.BucketWeight.YN.XN.1.weight",  // 20
					"Vehicle.Body.Load.BucketWeight.YN.XN.2.weight",  // 21
					"Vehicle.Body.Load.BucketWeight.YN.XN.3.weight",  // 22
					"Vehicle.Hardware.RemoteController.mode",  // 23
					"Vehicle.Hardware.RemoteController.LoadMode",  // 24
					"Vehicle.Hardware.RemoteController.ControlLoadMode", // 25
					"Vehicle.Hardware.RemoteController.Speed",  // 26
					"Vehicle.Hardware.RemoteController.SpeedDir",  // 27
					"Vehicle.Hardware.RemoteController.mission",  // 25
					"Vehicle.Hardware.RemoteController.missionDir"  // 25 // To Do
					// "Vehicle.Hardware.RemoteController.StAngle",  // 28 // To Do
					// "Vehicle.Hardware.RemoteController.StAngleDir"  // 29 // To Do

		};
		int initValue[26] = {110,0,70, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 1, 1, 1,0,1,0,1};
    	KukClient vehicle = KukClient();
		vehicle.set(sensorPaths, initValue, 26);
		vehicle.~KukClient();

}

int main(int argc, char** argv) {
		

		// init();
		BucketManagement b = BucketManagement();
		b.run();
	
        
	
	

	
	return 0;
}
