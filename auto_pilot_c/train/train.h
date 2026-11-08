#ifndef Train_H
#define Train_H
#endif
#include "points.h"
#ifndef  Database_H
#include "database.h"
#endif

#include "kuksa.grpc.pb.h"
#include "kclient.h"



#include "javaneh.grpc.pb.h"

using namespace Tiro;


class Train
{
	private:
		int count=0 ;
		char* dbname;
		Database* db;
		KukClient vehicle = KukClient();
		std::string paths[11] = {
					"Vehicle.Hardware.RemoteController.Speed",  // 0
					"Vehicle.Hardware.RemoteController.SpeedDir",  // 1
					"Vehicle.Hardware.RemoteController.StAngle", // 2
					"Vehicle.Hardware.RemoteController.StAngleDir", // 3
					"Vehicle.Hardware.RemoteController.isSafeMode", // 4
					"Vehicle.Hardware.RemoteController.isTurningPlace", // 5
					"Vehicle.Hardware.RemoteController.mission", // 6
					"Vehicle.Hardware.RemoteController.mode", // 7
					"Vehicle.Hardware.Surround.GPS.Back.lat",  // 8
					"Vehicle.Hardware.Surround.GPS.Back.lon", // 9
					"Vehicle.Hardware.Surround.CompassAngle" //  10
		};
		Points* points;
		Point point;
		int auto_pilot_mode = 0;
		int mission_id = 0;
		int line = 0;
		int iteration = 0;
		int last_point_id = 0;
		double lat;
		double lon;
	public:
		Train();
		Train(char* filename);
		void run();
		void getPoint();
		void start();
		void stop();
		void initTraining();
		void finishTraining();
		void setLine();
		void record();
		bool isMoving();
		void normalize();
};
