#include <iostream>
#include <string>
#include <grpc++/grpc++.h>
#ifndef  Canbus_H
#include "canbus.h"
#endif

#include "drive.h"


using namespace std;
using namespace Tiro;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


#define  AUTO_PILOT_CAN_ID    16
#define  DRIVE_MODE_CAN_ID    9
#define DRIVE_MODE_AUTO_PILOT_ON 1
#define DRIVE_MODE_AUTO_PILOT_OFF 0


Drive::Drive()
{
    
}

Drive::Drive(char* filename)
{
    dbname = filename;
    canbus = new Canbus("can0");
    AuthResponse r = vehicle->authorize();
    
}

void Drive::run()
{
    db = new Database();
    main_line_points = new Points(db);
    // right_line_points = new Points(db);
    // left_line_points = new Points(db);

    
    while(true){
        if(helper.getPoint(vehicle, point) == 0){
            usleep(1000000);
            continue;
        }
        
        int front_side = helper.vehicleSide(point->hardware_front_side(), 
                           point->software_front_side(), 
                           point->auto_pilot_mode());
        // printf("fronside : %i %i %i\n", 
        //         point->hardware_front_side(),
        //         point->software_front_side(),
        //         front_side);


        if(point->auto_pilot_mode() == 2){
            start();
        }else{
            stop();
        }
        usleep(100000);
		
	}
}

void Drive::start(){
    initDrive();
    drive();
}

void Drive::stop(){
    finishDrive();
}

// initialize new drive
void Drive::initDrive(){
    if(auto_pilot_mode == DRIVE_MODE_AUTO_PILOT_OFF){
        count = 1;
        usleep(500000);
        
        auto_pilot_mode = DRIVE_MODE_AUTO_PILOT_ON;

        db->connect(dbname, 1);
        mission_id = point->mission();
        Mission* mission = new Mission(); 
        db->readMission(mission, mission_id);
        main_line_iteration = db->readIteration(mission_id, 1);
        // right_line_iteration = db->readIteration(mission_id, 0);
        // left_line_iteration = db->readIteration(mission_id, 2);
        main_line_points->readPoints(mission_id, main_line_iteration);    
        // right_line_points->readPoints(mission_id, right_line_iteration);    
        // left_line_points->readPoints(mission_id, left_line_iteration);    
        sendCanDriveMode();
        printf("start driving\n");
    }
    
}

// finish the running training
void Drive::finishDrive(){
    if(auto_pilot_mode == DRIVE_MODE_AUTO_PILOT_ON){
        auto_pilot_mode = DRIVE_MODE_AUTO_PILOT_OFF;
        stopCar();
        sendCanDriveMode();
        main_line_points->cleanup();
        // right_line_points->cleanup();
        // left_line_points->cleanup();
        db->close();
        printf("finish driving\n");
    }
    
}
void Drive::drive(){
    
    double steering_angle = 0;
    double steering_angle_direction = 0;
    main_line_points->neighbors(main_neighbor_points, 3, point->lat(), point->lon());
    // right_line_points->neighbors(right_neighbor_points, 3, point->lat(), point->lon());
    // left_line_points->neighbors(left_neighbor_points, 3, point->lat(), point->lon());
    main_point = main_line_points->points[main_neighbor_points[0]];
    printf("id : %i\n", main_point.id());
    if(main_point.is_last()){
        stopCar();
        return;
    }
    double start_end_distance = main_line_points->distance(main_point.start_lat(),
                                        main_point.start_lon(),
                                        main_point.end_lat(),
                                        main_point.end_lon());

    // printf("start lat : %.8f,start lon : %.8f, end lat : %.8f, end lon : %.8f\n", main_point.start_lat(),
    //                                     main_point.start_lon(),
    //                                     main_point.end_lat(),
    //                                     main_point.end_lon());

    double start_current_distance = main_line_points->distance(main_point.start_lat(),
                                        main_point.start_lon(),
                                        point->lat(),
                                        point->lon());

    double end_current_distance = main_line_points->distance(main_point.end_lat(),
                                        main_point.end_lon(),
                                        point->lat(),
                                        point->lon());
    // printf("%.8f , %.8f, %.8f \n", start_end_distance, start_current_distance, end_current_distance);
    // printf("%.8f , %.8f, %.8f, %.8f\n",main_point.end_lat(),
    //                                     main_point.end_lon(),
    //                                     point->lat(),
    //                                     point->lon());
    double line_distance = main_line_points->lineDistance(start_end_distance, start_current_distance, end_current_distance);

    double start_end_heading = main_line_points->heading(main_point.start_lat(),
                                        main_point.start_lon(),
                                        main_point.end_lat(),
                                        main_point.end_lon());

    double end_current_heading = main_line_points->heading(main_point.end_lat(),
                                        main_point.end_lon(),
                                        point->lat(),
                                        point->lon());

    // printf("line distance : %.8f , ec distance : %.8f , sc distance : %.8f \n", 
    //         line_distance, 
    //         end_current_distance, 
    //         start_current_distance);

    if(line_distance < 0.00002 || end_current_distance == 0){
        steering_angle = point->steering_angle();
        steering_angle_direction = point->steering_angle_direction();
    }
    else{
        steering_angle = round(1500*line_distance/end_current_distance);
        steering_angle += point->steering_angle();
        double angle_diff = abs(start_end_heading-point->angle());
        if(line_distance < (0.00001 * angle_diff)){
            steering_angle *= angle_diff/10;
        }    

    }
    
    if(steering_angle > 255){
            steering_angle = 255;
    }

    double heading = 0;
    if(lat != 0){
        heading = main_line_points->heading(lat, lon, point->lat(), point->lon());
    }
    lat = point->lat();
    lon = point->lon();
    

    int direction = calculateDirection(start_end_heading, end_current_heading, line_distance);

    printf("ld : %.8f , steer : %f, dir : %i , lh : %.2f, ch : %.2f, cmps: %i\n", 
            line_distance*100000, 
            // end_current_distance*100000, 
            // start_current_distance*100000, 
            steering_angle,
            direction, 
            start_end_heading,
            heading,
            point->angle());


    int canmsg[8] = {0};
    canmsg[0] = main_point.speed(); // speed;
    canmsg[1] = main_point.speed_direction();
    // canmsg[2] = main_point.steering_angle();
    canmsg[2] = steering_angle;
    // canmsg[3] = main_point.steering_angle_direction();
    canmsg[3] = direction;
    canmsg[4] = main_point.safe_mode();
    canmsg[5] = main_point.turn_in_place();
    canbus->send(AUTO_PILOT_CAN_ID, canmsg);
    
    // printf("%i lat %.8f, lon : %.8f, l distance : %.8f , l heading : %.3f, c heading : %.3f dir: %i\n", 
    //         count, 
    //         point->lat(), 
    //         point->lon(),
    //         line_distance,
    //         start_end_heading,
    //         end_current_heading, 
    //         direction);
    count++;
    
}
int Drive::calculateDirection(double start_end_heading, double end_current_heading, double line_distance){
    double diff = 0;
    int direction;
    // if(line_distance < 0.00015){
    //         diff = end_current_heading - point->angle();
    // }else{
    //         diff = end_current_heading - start_end_heading;
    // }
    diff = end_current_heading - point->angle();
    // if(line_distance < (0.00001 * angle_diff)){
    //     steering_angle *= angle_diff/10;
    // }   
    if ((diff > 0 && diff < 180) || (diff < -180 && diff > -360)){
        direction = 0; // right;
    }else{
        direction = 1; // left
    }

    // if(line_distance < (0.00001 * angle_diff)){
    //     steering_angle *= angle_diff/10;
    // }

    return direction;
}

void Drive::stopCar(){
    int canmsg[8] = {0};
    canbus->send(AUTO_PILOT_CAN_ID, canmsg);
}

void Drive::sendCanDriveMode(){
    int canmsg[8] = {0};
    canmsg[0] = auto_pilot_mode;
    canbus->send(DRIVE_MODE_CAN_ID, canmsg);
}