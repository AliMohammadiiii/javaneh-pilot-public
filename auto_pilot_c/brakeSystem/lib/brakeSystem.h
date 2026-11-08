#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <kclient.h>
#include "../etc/brakeRow.h"
#include <unistd.h> 
// #include "kuksa.grpc.pb.h"

using namespace std;

// using kuksa::AuthResponse;
// using kuksa::GetResponse;

#define NUMBEROFROWS 4
#define NUMBEROFSENSOR 18
#define PARK 0

#define PATH_PARKBRAKE_INNER_YPXP "Vehicle.Corner.YP.XP.Brake.Inner.ParkValve"
#define PATH_BRAKEBRAKE_INNER_YPXP "Vehicle.Corner.YP.XP.Brake.Inner.BrakeValve"
#define PATH_RELEASEBRAKE_INNER_YPXP "Vehicle.Corner.YP.XP.Brake.Inner.ReleaseValve"


#define PATH_PARKBRAKE_OUTER_YPXP "Vehicle.Corner.YP.XP.Brake.Outer.ParkValve"
#define PATH_BRAKEBRAKE_OUTER_YPXP "Vehicle.Corner.YP.XP.Brake.Outer.BrakeValve"
#define PATH_RELEASEBRAKE_OUTER_YPXP "Vehicle.Corner.YP.XP.Brake.Outer.ReleaseValve"

#define PATH_PARKBRAKE_INNER_YPXN "Vehicle.Corner.YP.XN.Brake.Inner.ParkValve"
#define PATH_BRAKEBRAKE_INNER_YPXN "Vehicle.Corner.YP.XN.Brake.Inner.BrakeValve"
#define PATH_RELEASEBRAKE_INNER_YPXN "Vehicle.Corner.YP.XN.Brake.Inner.ReleaseValve"

#define PATH_PARKBRAKE_OUTER_YPXN "Vehicle.Corner.YP.XN.Brake.Outer.ParkValve"
#define PATH_BRAKEBRAKE_OUTER_YPXN "Vehicle.Corner.YP.XN.Brake.Outer.BrakeValve"
#define PATH_RELEASEBRAKE_OUTER_YPXN "Vehicle.Corner.YP.XN.Brake.Outer.ReleaseValve"

#define PATH_PARKBRAKE_INNER_YNXP "Vehicle.Corner.YN.XP.Brake.Inner.ParkValve"
#define PATH_BRAKEBRAKE_INNER_YNXP "Vehicle.Corner.YN.XP.Brake.Inner.BrakeValve"
#define PATH_RELEASEBRAKE_INNER_YNXP "Vehicle.Corner.YN.XP.Brake.Inner.ReleaseValve"

#define PATH_PARKBRAKE_OUTER_YNXP "Vehicle.Corner.YN.XP.Brake.Outer.ParkValve"
#define PATH_BRAKEBRAKE_OUTER_YNXP "Vehicle.Corner.YN.XP.Brake.Outer.BrakeValve"
#define PATH_RELEASEBRAKE_OUTER_YNXP "Vehicle.Corner.YN.XP.Brake.Outer.ReleaseValve"

#define PATH_PARKBRAKE_INNER_YNXN "Vehicle.Corner.YN.XN.Brake.Inner.ParkValve"
#define PATH_BRAKEBRAKE_INNER_YNXN "Vehicle.Corner.YN.XN.Brake.Inner.BrakeValve"
#define PATH_RELEASEBRAKE_INNER_YNXN "Vehicle.Corner.YN.XN.Brake.Inner.ReleaseValve"

#define PATH_PARKBRAKE_OUTER_YNXN "Vehicle.Corner.YN.XN.Brake.Outer.ParkValve"
#define PATH_BRAKEBRAKE_OUTER_YNXN "Vehicle.Corner.YN.XN.Brake.Outer.BrakeValve"
#define PATH_RELEASEBRAKE_OUTER_YNXN "Vehicle.Corner.YN.XN.Brake.Outer.ReleaseValve"


#define YPOUTER 0
#define YPINNER 1
#define YNOUTER 2
#define YNINNER 3



class brakeSystem
{
private:
    bool loadData();
    void calculator();
    void selectBrakePark();
    void selectBrakeService();
    void sendData();
    void setBrakesPath();
    void roundBrakeC();
    void calcNumberOfBrakes();
    int isOverHeat();
    bool isMoving();

    BrakeRow* findbrakeRow(int id);
    
    bool isLoggingMode;
    vector<string> changedPath;
    vector<int> changedValue;
    KukClient vehicle;
    BrakeRow* brakeRows[NUMBEROFROWS];
    // vector<BrakeRow> brakeRows;
    string path[NUMBEROFSENSOR] = {
            "Vehicle.Corner.YP.XP.Brake.Outer.CaliperTemp", // 2
            "Vehicle.Corner.YP.XN.Brake.Outer.CaliperTemp", // 4
            "Vehicle.Corner.YP.XP.Brake.Inner.CaliperTemp", // 1
            "Vehicle.Corner.YP.XN.Brake.Inner.CaliperTemp", // 3
            "Vehicle.Corner.YN.XP.Brake.Outer.CaliperTemp", // 6
            "Vehicle.Corner.YN.XN.Brake.Outer.CaliperTemp", // 8
            "Vehicle.Corner.YN.XP.Brake.Inner.CaliperTemp", // 7
            "Vehicle.Corner.YN.XN.Brake.Inner.CaliperTemp", // 5
            "Vehicle.Hardware.RemoteController.parkMode", // 9
            "Vehicle.Hardware.RemoteController.BrakeCommand", // 10 Change to brake control address
            "Vehicle.Corner.YP.XP.Wheel.Inner.TireRotation",
            "Vehicle.Corner.YP.XP.Wheel.Outer.TireRotation",
            "Vehicle.Corner.YP.XN.Wheel.Inner.TireRotation",
            "Vehicle.Corner.YP.XN.Wheel.Outer.TireRotation",
            "Vehicle.Corner.YN.XP.Wheel.Inner.TireRotation",
            "Vehicle.Corner.YN.XP.Wheel.Outer.TireRotation",
            "Vehicle.Corner.YN.XN.Wheel.Inner.TireRotation",
            "Vehicle.Corner.YN.XN.Wheel.Outer.TireRotation"
    };
    string rotationSpeed[8]
    {
            "Vehicle.Corner.YP.XP.Wheel.Inner.TireRotation",
            "Vehicle.Corner.YP.XP.Wheel.Outer.TireRotation",
            "Vehicle.Corner.YP.XN.Wheel.Inner.TireRotation",
            "Vehicle.Corner.YP.XN.Wheel.Outer.TireRotation",
            "Vehicle.Corner.YN.XP.Wheel.Inner.TireRotation",
            "Vehicle.Corner.YN.XP.Wheel.Outer.TireRotation",
            "Vehicle.Corner.YN.XN.Wheel.Inner.TireRotation",
            "Vehicle.Corner.YN.XN.Wheel.Outer.TireRotation"
    };
    int rotationSpeedValue[8] = {0,0,0,0,0,0,0,0};
    int brakeC = 0;
    int parkC = 0;
    int numberOfServiceBrake = 0;
    int prevNumberOfServiceBrake = 0;
    int numberOfParkBrake = 0;
    int prevNumberOfParkBrake = 0;

public:
    brakeSystem(/* args */);
    void run();
};
