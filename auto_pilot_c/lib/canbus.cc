#include <iostream>
// #include "javaneh.grpc.pb.h"
#include <math.h>


#include <stdio.h>
#include <sys/socket.h>  // i tried linux/socket.h and get some other errors, so I am confused.
#include <net/if.h>
#include <string.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#ifndef AF_CAN 
#define AF_CAN 29
#endif

#include <canbus.h>

using namespace std;
// using namespace Tiro;


Canbus::Canbus(){}
Canbus::Canbus(string interface){
	connect(interface);
}
int Canbus::connect(string interface){
	int count = 1000;
	struct sockaddr_can addr;
	struct ifreq ifr;

	// Openning the socket to send commands over the can bus
	if ((socket_id = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("[Can Controler] Unable to create socket.");
    		return false;

	}
	strcpy(ifr.ifr_name, interface.c_str());
	ioctl(socket_id, SIOCGIFINDEX, &ifr);
	memset(&addr, 0, sizeof(addr));

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;


	// can_err_mask_t err_mask = 0xFFFFFFFF;
	// setsockopt(socket_id, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));

	// struct timeval tv;
	// tv.tv_sec = 0;
	// tv.tv_usec = 10000;
	// setsockopt(socket_id, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);


	if (bind(socket_id, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("[Can Controler] Unable to bind socket.");
		close(socket_id);
		return false;
	}
  	return true;


}
void Canbus::readBus(){
	while(true){
		// continue;
		struct can_frame frame = can_frame();
		int msg_size = read(socket_id, &frame, sizeof(struct can_frame));
		// read(socket_id, &frame, sizeof(struct can_frame));
		if (msg_size < 0) {
			continue;
		}
		// process(&frame);
		
		
	}
	
}
can_frame Canbus::readMessage(){
	while(true){
		struct can_frame frame = can_frame();
		int msg_size = read(socket_id, &frame, sizeof(struct can_frame));
		if (msg_size < 0) {
			continue;
		}
		return frame;
		
		
	}
	
}
// void Canbus::process(can_frame* frame){
// 	if(frame->can_id == 17){ // get remote control data
// 		point->set_speed(frame->data[0]);
// 		point->set_speed_direction(frame->data[1]);
// 		point->set_steering_angle(frame->data[2]);
// 		point->set_steering_angle_direction(frame->data[3]);
// 		point->set_safe_mode(frame->data[4]);
// 		point->set_turn_in_place(frame->data[5]);
// 		point->set_auto_pilot_mode(frame->data[6]);
// 		point->set_mission(frame->data[7]);
// 	}
// 	else if(frame->can_id == 7){ // get compass
// 		point->set_angle((frame->data[0]*256) + frame->data[1]);
// 	}
// }
void Canbus::send(int id, int data[8]){
	struct can_frame frame;
	frame.can_id = id;
	frame.can_dlc = 8;
	frame.data[0] = data[0];
	frame.data[1] = data[1];
	frame.data[2] = data[2];
	frame.data[3] = data[3];
	frame.data[4] = data[4];
	frame.data[5] = data[5];
	frame.data[6] = data[6];
	frame.data[7] = data[7];
	// printf("send can msg\n");
	if (write(socket_id, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write");
		return;
	}
}