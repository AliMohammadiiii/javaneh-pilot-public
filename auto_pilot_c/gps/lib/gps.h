#ifndef Gps_H
#define Gps_H
#endif
#include <iostream>
#include <thread>
#include <libserial/SerialPort.h>
#include "kuksa.grpc.pb.h"
#include "kclient.h"

using namespace std;
using namespace LibSerial ;

class Gps 
{
	private:
		SerialPort serial_port;
		double lat;
		double lon;
		int fix;
		KukClient vehicle = KukClient();
		std::string paths[2] = {"Vehicle.Hardware.Surround.GPS.YN.lat", "Vehicle.Hardware.Surround.GPS.YN.lon"};
		double values[2] = {0.0,0.0};
		void process(char* msg);
	public:
		Gps(){}
		Gps(string device);
		void run();
		void update_kuksa();
};
