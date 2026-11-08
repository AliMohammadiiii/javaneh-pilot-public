#ifndef Lidar_H
#define Lidar_H
#endif
#include "points.h"
#ifndef  Canbus_H
#include "canbus.h"
#endif
#include "sl_lidar.h" 
#include "sl_lidar_driver.h"
#include <thread>
#include "javaneh.grpc.pb.h"
#include "kuksa.grpc.pb.h"
#include "kclient.h"
using namespace sl;
using namespace Tiro;


class Lidar
{
	private:
		char* device_name[2];
		int budrate;
		int connect_status=0; // 0 not connected, 1 connected
		int status=-1;  // not set -1 , 1 start , 0 stop
		float width_car=40;
    	float margin_car=20;
    	float allowedـdistance=100;
		int range=90;
		int direction;
		int _direction=-1;
        int speed;
		GetResponse g1;
		sl_result     op_result;
		ILidarDriver * drv[2];
		Canbus* canbus;
		Point point;
		KukClient vehicle = KukClient();
		std::string paths[2] = {
					"Vehicle.Hardware.RemoteController.Speed",  // 0
					"Vehicle.Hardware.RemoteController.SpeedDir"  // 1

		};

	public:
		Lidar();
		Lidar(char* device1,char* device2, int rate);
		void connect();
		void start(ILidarDriver* drv);
		void stop(ILidarDriver* drv);
		void scan(ILidarDriver* drv);
		float caldangerousdist(float angle);
		void getdirandspeed();
		void runlidar();
		void run();
		
};
