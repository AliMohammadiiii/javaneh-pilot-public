#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>

#include <kclient.h>

#include "kuksa.grpc.pb.h"

using namespace std;

using kuksa::AuthResponse;
using kuksa::GetResponse;


#define MOTOR_R 0
#define MOTOR_L 1
#define SPEED 0
#define DIRECTION 1
#define ID_CAN_MOTOR 4
#define RADIUS 15
#define WIDTH 30
#define WHEELBASELEN 32
#define MINIMUM_SPEED_FAST_TURN 4
#define MINIMUM_DISTANCE 25
#define MAX_RPM 100
#define MAX_VOLTAGE_MOTOR 220
#define ID_CAN_MOVEMENT 20
#define ID_LAYER_PLANNING 2
#define ID_CHANGE_SPEED 21
#define ID_CHANGE_ANGLE 22
#define ID_BREAK 23
#define ID_ULTRASONIC 40
#define ID_LASER 5
#define OUTERHIGHMULTIPLICANT 1.4
#define INNERHIGHMULTIPLICANT 0.6

#define OUTERMULTIPLICANT 1.2
#define INNERMULTIPLICANT 0.8
#define STOP 0
#define P 1
#define N 0
#define AUTOPILOTMODE 0
uint64_t timeSinceEpochMillisec();

class controllerRationalSteering
{
private:
    KukClient vehicle;
    double trackIn, trackOut, wheelBaseIn, wheelBaseOut, tireDia, radiusMin;
	double rpmAverage,angle, vehicleSpeed, raduis, wheelRadius[2], rpm[2], deg, turnDirection, wheelDirection, stop, radiusMultiplicant[2];
    int brakeC;
    int motorValue[8][2];
	int isItturnInPlace, isSafeMode;
    int frontSide;
    uint64_t lastSend;
    int autoPilotMode;
    vector<string> autoPilotModePath;
    vector<string> radioPath;
    vector<string> autoPilotPath;
    string motorPath[8][2];

    vector <string> changedPath;
    vector<int> changedValue;
    void loadData();
    void sendData();

    void radiusCalculation();
    // Calculate the radius of the wheels based on the robot's radius and the steering direction.
    void wheelRadiusCalculation();
    double overFlowRpm();
    // Correcting the wheel coefficient
    void radiusMultiplicantCallculator(); 
    // Calculate the RPM of the wheels based on the robot's speed and radius.
    void rpmCalculator();


    void setVehicleSpeed(double s, int direction);
    void setAngle(double angle, int direction);


    void motorValueCalculation();
    void forceStop(int state);
    void speedCalculation();
    void turnInPlace();

    void turningInPlaceState(int state);
    void safeMode(int state);
    void setChangedValue(int motorID, int type, int value);


public:
    controllerRationalSteering(/* args */);
    ~controllerRationalSteering();
    void run();
};


