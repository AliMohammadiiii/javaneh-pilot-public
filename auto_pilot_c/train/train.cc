#include <iostream>
#include <string>
#include <grpc++/grpc++.h>
#include "train.h"

using namespace std;
using namespace Tiro;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

#define  MOVING_THRESHOLD    0.00001   // in KM


Train::Train()
{
    
}

Train::Train(char* filename)
{
    dbname = filename;
    AuthResponse r = vehicle.authorize();
    
    
}

void Train::getPoint(){
    
    GetResponse g1 = vehicle.get(paths, 11);
    point.set_speed(g1.values(0).valueint32());
    point.set_speed_direction(g1.values(1).valueint32());
    point.set_steering_angle(g1.values(2).valueint32());
    point.set_steering_angle_direction(g1.values(3).valueint32());
    point.set_mission(g1.values(6).valueint32());
    point.set_auto_pilot_mode(g1.values(7).valueint32());
    point.set_lat(g1.values(8).valuedouble());
    point.set_lon(g1.values(9).valuedouble());
    point.set_angle(g1.values(10).valueint32());
    
    
}
void Train::run()
{
    db = new Database();
    while(true){
        getPoint();
        if(point.auto_pilot_mode() == 1){
            start();
        }else{
            stop();
        }
		usleep(5000);
	}
}

void Train::start(){
    initTraining();
    record();
}

void Train::stop(){
    finishTraining();
}

// initialize new training
void Train::initTraining(){
    if(auto_pilot_mode == 0){
        printf("starting new training \n");
        usleep(2000000);
        count = 1;
        
        db->connect(dbname, 0);
        points = new Points();
        auto_pilot_mode = 1;
        mission_id = point.mission();
        Mission* mission = new Mission(); 
        db->readMission(mission, mission_id);
        setLine();
        iteration = db->addIteration(mission_id, line);
        printf("start training mission : %i, line : %i, iteration : %i\n", mission_id, line, iteration);


    }
    
}

// finish the running training
void Train::finishTraining(){
    if(auto_pilot_mode == 1){
        
        auto_pilot_mode = 0;
        db->setLastPoint(last_point_id);
        normalize();
        db->close();
        printf("finish training, number of records : %i\n", count);
    }
    
}

void Train::setLine(){
    if(point.steering_angle() < 125){
        line = 1; 
    }else if(point.steering_angle_direction() == 1){
        line = 2; 
    }
    else{
        line = 0; 
    }
}

void Train::record(){
    // printf("new record\n");
    if(isMoving()){
        point.set_mission(mission_id);
        point.set_line(line);
        point.set_iteration(iteration);
        last_point_id = db->newPoint(&point);
        // printf("%i : last point id : %i\n", count, last_point_id);
        count++;
    }

}

bool Train::isMoving(){
    if(lat != 0){
        double lat2 = point.lat();
        double lon2 = point.lon();
        double distance = points->distance(lat, lon, point.lat(), point.lon());
        if(distance > MOVING_THRESHOLD){
            lat = point.lat();
            lon = point.lon();
            return true;
        }
    }
    return false;
}
void Train::normalize(){
    Points* ps = new Points(db);
    ps->readPoints(mission_id, iteration);
    int normalize_threshold = 5;
    int steering_angle = -1 * (normalize_threshold+1);
    int steering_angle_direction = -1;
    int start_point = -1;
    int end_point = -1;
    // points classification based on steering angle
    for(int i=0; i < ps->count; i++){
        // change the points class if steering angle direction changed , 
        if(steering_angle_direction != ps->points[i].steering_angle_direction() ||
        // change the points class if steering angle changes bigger than normalize threshold , 
         abs(ps->points[i].steering_angle() - steering_angle) > normalize_threshold ||
         // if steering angle is larger than 5 , use steering angle value to determine points class memmbers count
         (ps->points[i].steering_angle() > 5 && (end_point - start_point > ((300 - ps->points[i].steering_angle())/10)))
         ){
            if(start_point != -1){
                db->normalize(iteration, ps->points[start_point], ps->points[i]);
            }
            steering_angle = ps->points[i].steering_angle();
            steering_angle_direction = ps->points[i].steering_angle_direction();
            start_point = i;
        }
        end_point = i;
    }
    db->normalize(iteration, ps->points[start_point], ps->points[end_point]);
}
