#include "helper.h"
#include <iostream>
// #include "javaneh.grpc.pb.h"
using namespace std::chrono;
#include <chrono>






// using namespace std;
// using namespace Tiro;

Helper::Helper(){}

int Helper::vehicleSide(int hardware_front_side, int software_front_side, int auto_pilot_mode){
	if(auto_pilot_mode != 2){
		return hardware_front_side;
	}else{
		return software_front_side;
	}
}
uint64_t Helper::timeSinceEpochMillisec() {
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
// int Helper::getPoint(KukClient* vehicle, Point* point){
//     GetResponse g1 = vehicle->get(points_paths, 13);
// 	if(vehicle->connection_state == 2 && g1.values_size() == 13){
// 		point->set_speed(g1.values(0).valueint32());
// 		point->set_speed_direction(g1.values(1).valueint32());
// 		point->set_steering_angle(g1.values(2).valueint32());
// 		point->set_steering_angle_direction(g1.values(3).valueint32());
// 		point->set_mission(g1.values(6).valueint32());
// 		point->set_auto_pilot_mode(g1.values(7).valueint32());
// 		point->set_lat(g1.values(8).valuedouble());
// 		point->set_lon(g1.values(9).valuedouble());
// 		point->set_angle(g1.values(10).valueint32());    
// 		point->set_hardware_front_side(g1.values(11).valueint32());    
// 		point->set_software_front_side(g1.values(12).valueint32());    
// 		return 1;
// 	}
// 	return 0;
// }
