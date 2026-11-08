#include <iostream>
#include <string>
#include <grpc++/grpc++.h>
#include "vehicle_client.h"
#ifndef  Canbus_H
#include "canbus.h"
#endif
#include "lidar.h"
#include <thread>
#include "sl_lidar.h" 
#include "sl_lidar_driver.h"

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

using namespace std;
using namespace Tiro;
using namespace sl;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
#define PI 3.14159265
Lidar::Lidar()
{
    
}

Lidar::Lidar(char* device1,char* device2, int rate)
{
    device_name[0] = device1;
    device_name[1] = device2;
    budrate = rate;
    connect();
    AuthResponse r = vehicle.authorize(); 
}
void Lidar::connect()
{
    drv[0] = *createLidarDriver();
    drv[1] = *createLidarDriver();
    IChannel* _channel = (*createSerialPortChannel(device_name[0], budrate));
    (drv[0])->connect(_channel);
    if (SL_IS_OK((drv[0])->connect(_channel))) {
        connect_status = 1;
    }
    _channel = (*createSerialPortChannel(device_name[1], budrate));
    (drv[1])->connect(_channel);
    if (SL_IS_OK((drv[1])->connect(_channel))) {
        connect_status = 1;
    }
}
void Lidar::start(ILidarDriver* drv)
{
    if (connect_status) {
        drv->setMotorSpeed();
        drv->startScan(0,1);
        status = 1;
    }
}
void Lidar::stop(ILidarDriver* drv)
{
    drv->stop();
    drv->setMotorSpeed(0);
    status = 0;
}
void Lidar::scan(ILidarDriver* drv)
{
        sl_lidar_response_measurement_node_hq_t nodes[8192];
        size_t   count = _countof(nodes);
        float min=1000000.0;
        float max=0.0;
        float min_angle;
        float angle;
        float d;
        int c=0;
        op_result = drv->grabScanDataHq(nodes, count);
        if (SL_IS_OK(op_result)) {
            drv->ascendScanData(nodes, count);
            for (int pos = 0; pos < (int)count ; ++pos) {
                angle=(nodes[pos].angle_z_q14 * 90.f) / 16384.f;
                if(angle <range || angle >(360-range))
                {
                    d=nodes[pos].dist_mm_q2/4.0f;
                    if(d == 0.0){
                        continue;
                    }
                    if(min>d){
                        min=d;
                        min_angle=angle;
                    }
                    if(max<d){
                        max=d;
                    }
                    if(caldangerousdist(angle)>d/10){
                        c++;
                    }
                }
            }
            printf("Min: %08.2f Min_angle: %03.2f Max: %08.2f  count: %i caldangerousdist: %f \n",min,min_angle,max,c,caldangerousdist(angle));
        }
}
float Lidar::caldangerousdist(float angle)
{
    float dangerous_dist_perAngle;
    float max_atan;
    if(angle>270){
        angle=360-angle;
    }
    max_atan=atan (((width_car/2)+margin_car)/allowedـdistance)* 180 / PI;;
    if(angle>max_atan)
        dangerous_dist_perAngle=((width_car/2)+margin_car)/ sin(angle*3.14159/180);
    else
        dangerous_dist_perAngle=(allowedـdistance)/ cos(angle*3.14159/180);
    //printf("angle= %f dangerous_dist= %f\n----------------------\n",angle,dangerous_dist_perAngle);
    return(dangerous_dist_perAngle);
}
void Lidar::getdirandspeed(){
    while (true)
    {
        g1 = vehicle.get(paths,2);
        direction = g1.values(1).valueint32();
        speed = g1.values(0).valueint32();


        
        usleep(500000);
    }
}
void Lidar::runlidar(){

            printf("direction= %i speed= %i\n----------------------\n",direction,speed);
    while (true)
    {
        if(speed>80){
            if(_direction!=direction){
                // printf("direction: %i speed: %i  \n",direction,speed);
                if(_direction!= -1){
                    // printf("stop1 lidar %i \n", _direction);
                    stop(drv[_direction]);
                    
                }
                // printf("start lidar %i \n", direction);
                start(drv[direction]);
                _direction = direction;
            }
            scan(drv[direction]);
        }
        else if(_direction!=-1){
            // printf("stop2 lidar %i \n", _direction);
            stop(drv[_direction]);
            _direction=-1;
        }
    }
}
void Lidar::run()
{
    thread vehicle_thread(&Lidar::getdirandspeed, this);
    thread lidar_thread(&Lidar::runlidar, this);
    vehicle_thread.join();
    lidar_thread.join();
}


