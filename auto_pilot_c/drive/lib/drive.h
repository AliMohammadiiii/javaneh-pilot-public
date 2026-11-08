#ifndef Drive_H
#define Drive_H
#endif
#include "points.h"
#ifndef  Database_H
#include "database.h"
#endif
#ifndef  Canbus_H
#include "canbus.h"
#endif

#include "kuksa.grpc.pb.h"

#ifndef  KClient_H
#include "kclient.h"
#endif
#include "helper.h"




#include "javaneh.grpc.pb.h"

using namespace Tiro;


class Drive
{
	private:
		char* dbname;
		Database* db;
		KukClient* vehicle = new KukClient();
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
		Helper helper = Helper();
		Canbus* canbus;
		Points* main_line_points;
		Points* right_line_points;
		Points* left_line_points;
		Point* point = new Point();
		Point main_point;
		int count;
		int auto_pilot_mode = 0;
		int mission_id = 0;
		int line = 0;
		int main_line_iteration = 0;
		int left_line_iteration = 0;
		int right_line_iteration = 0;
		int main_neighbor_points[3] = {0};
    	int right_neighbor_points[3] = {0};
    	int left_neighbor_points[3] = {0};
		double lat=0;
		double lon=0;
		// void getPoint();


	public:
		Drive();
		Drive(char* filename);
		void run();
		void start();
		void stop();
		void initDrive();
		void finishDrive();
		void drive();
		int calculateDirection(double start_end_heading, double end_current_heading, double line_distance);
		void stopCar();
		void sendCanDriveMode();
		
};
