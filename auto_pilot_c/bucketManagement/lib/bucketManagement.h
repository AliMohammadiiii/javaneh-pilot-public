#ifndef BUCKETMANAGEMENT_H
#define BUCKETMANAGEMENT_H

#include <iostream>
#include <string.h>
#include <algorithm> 
#include <vector>
#include <kclient.h>
// #include "kuksa.grpc.pb.h"
#include <limits>
#include <unistd.h> 

using namespace std;
// using kuksa::AuthResponse;
// using kuksa::GetResponse;/

#define PSIDE 1 
#define NSIDE 2
#define RSIDE 3
#define LSIDE 4

#define PARK 0
#define DRIVE 1

#define RAISE 0
#define STOPCONTROLLER 1
#define DECREASE 2

#define STOP 0

#define YP 0
#define YN 1

#define XP 0
#define XN 1

#define HYDRLICMOTOR1ACTIVE 1
#define HYDRLICMOTOR2ACTIVE 0
#define RELEAFEVALVETURNOFFTHR 8

#define THRANGLESIDE 10
#define THRANGLEZERO 0.5

#define MAXANGLEX 67
#define MAXANGLEY 60
class BucketManagement
{
private:
    KukClient vehicle;
    string sensorPaths[30] = {
					"Vehicle.Body.Load.BucketAngle.x",  // 0
					"Vehicle.Body.Load.BucketAngle.y",  // 1
					"Vehicle.Frame.Chassis.ChassisAngle.x",  // 2
					"Vehicle.Frame.Chassis.ChassisAngle.y",  // 3
					"Vehicle.Energy.Hydrolic.OilPumpPressure",  // 4
					"Vehicle.Energy.Hydrolic.OilPumpTemp",  // 5
					"Vehicle.Energy.Hydrolic.OilTankLevel",  // 6
					"Vehicle.Body.Load.BucketWeight.YP.XP.0.weight",  // 8
					"Vehicle.Body.Load.BucketWeight.YP.XP.1.weight",  // 9
					"Vehicle.Body.Load.BucketWeight.YP.XP.2.weight",  // 10
					"Vehicle.Body.Load.BucketWeight.YP.XP.3.weight",  // 11
					"Vehicle.Body.Load.BucketWeight.YP.XN.0.weight",  // 12
					"Vehicle.Body.Load.BucketWeight.YP.XN.1.weight",  // 13
					"Vehicle.Body.Load.BucketWeight.YP.XN.2.weight",  // 14
					"Vehicle.Body.Load.BucketWeight.YP.XN.3.weight",  // 15
					"Vehicle.Body.Load.BucketWeight.YN.XP.0.weight",  // 16
					"Vehicle.Body.Load.BucketWeight.YN.XP.1.weight",  // 17	
					"Vehicle.Body.Load.BucketWeight.YN.XP.2.weight",  // 18
					"Vehicle.Body.Load.BucketWeight.YN.XP.3.weight",  // 19
					"Vehicle.Body.Load.BucketWeight.YN.XN.0.weight",  // 20
					"Vehicle.Body.Load.BucketWeight.YN.XN.1.weight",  // 21
					"Vehicle.Body.Load.BucketWeight.YN.XN.2.weight",  // 22
					"Vehicle.Body.Load.BucketWeight.YN.XN.3.weight",  // 23
					"Vehicle.Hardware.RemoteController.parkMode",  // 24
					"Vehicle.Hardware.RemoteController.LoadMode",  // 25
					"Vehicle.Hardware.RemoteController.ControlLoadMode", // 26
					"Vehicle.Hardware.RemoteController.Speed",  // 27
					"Vehicle.Hardware.RemoteController.BrakeCommand",  // 28
					"Vehicle.Hardware.RemoteController.mission",  // 29
					"Vehicle.Hardware.RemoteController.missionDir"  // 30 // To Do
					// "Vehicle.Hardware.RemoteController.StAngle",  // 28 // To Do
					// "Vehicle.Hardware.RemoteController.StAngleDir"  // 29 // To Do

	};

	string hydrolicJacValveOpen[2][2] =
	{
		{
			"Vehicle.Corner.YP.XP.HydrolicJac.Open.valve",
			"Vehicle.Corner.YP.XN.HydrolicJac.Open.valve"
		},
		{
			"Vehicle.Corner.YN.XP.HydrolicJac.Open.valve",
			"Vehicle.Corner.YN.XN.HydrolicJac.Open.valve"
		}
	};
	int hydrolicJacValveOpenValue[2][2];
	string hydrolicJacValveClose[2][2] =
	{
		{
			"Vehicle.Corner.YP.XP.HydrolicJac.Close.valve",
			"Vehicle.Corner.YP.XN.HydrolicJac.Close.valve"
		},
		{
			"Vehicle.Corner.YN.XP.HydrolicJac.Close.valve",
			"Vehicle.Corner.YN.XN.HydrolicJac.Close.valve"
		}
	};
	string hydrolicMotor[2] = 
	{
		"Vehicle.Energy.Hydrolic.HydraulicMotor.1",
		"Vehicle.Energy.Hydrolic.HydraulicMotor.2"
	};
	int hydrolicMotorValue[2];
	string releaf = "Vehicle.Energy.Hydrolic.ReleafValve";
	string flowControl = "Vehicle.Energy.Hydrolic.flowControl";
	int releafValue;
	int flowControlValue;

	int hydrolicJacValveCloseValue[2][2];
    int chassisAngleX, chassisAngleY;
    int bucketAngleX, bucketAngleY;
    int oilPumpP, oilPumpT, oilTankL;
    int bucketWeight[2][2][4];
	int parkMode;
	int LoadMode;
	int ControllLoadMode;
	int sideUp;
	int prev_sideUp;
	int logLevel;
	vector<string> pathChanged;
	vector<int> valueChanged;
	void changeValveValue(int &value, string &address, int newValue);
	void raiseMode();
	void backToNaturalPos();
	void ForcebackToNaturalPos();
    bool loadData();
    bool loadDataLogger();
	void controller();
	void setAllToZero(string hydrolicJacValve[2][2], int hydrolicJacValveValue[2][2]);
	bool isProperForUnloading();

public:
    BucketManagement();
    void run();
};



#endif