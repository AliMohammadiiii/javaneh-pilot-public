#ifndef Canbus_H
#define Canbus_H
#endif
#include <iostream>
#include <linux/can.h>
// #include "javaneh.grpc.pb.h"
#include <unistd.h>
using namespace std;
// using namespace Tiro;


class Canbus 
{
	public:
		int socket_id;
		// Point *point;
		Canbus();
		Canbus(string device);
		int connect(string device);
		void readBus();
		can_frame readMessage();
		void process(can_frame* frame);
		void send(int id, int data[8]);
};
