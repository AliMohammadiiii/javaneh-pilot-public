#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <kclient.h>
#include "kuksa.grpc.pb.h"

using namespace std;

using kuksa::AuthResponse;
using kuksa::GetResponse;

#define NUMBEROFROWS 4
#define YP 0
#define YN 1
#define XP 0
#define XN 1
#define NUMBEROFSENSOR 1
#define YPFRONTSIDE 1
#define YNFRONTSIDE 2

class utilityControl
{
private:
    bool loadData();
    void sendData();
    void calculator();

    bool isLoggingMode;
    vector<string> changedPath;
    vector<int> changedValue;
    KukClient vehicle;
    
    int headLight[2][2];
    int frontSide;
    string actuatorPath[4]
    {
        "Vehicle.Body.Head.YP.XP.Light.Head",
        "Vehicle.Body.Head.YP.XN.Light.Head",
        "Vehicle.Body.Head.YN.XP.Light.Head",
        "Vehicle.Body.Head.YN.XN.Light.Head"
    };

    string sensorPath[1]
    {
        "Vehicle.Hardware.RemoteController.FrontSide"
    };

public:
    utilityControl(/* args */);
    void run();
};
