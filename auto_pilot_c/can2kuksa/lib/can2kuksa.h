#ifndef CAN2KUKSA_H
#define CAN2KUKSA_H

#include <iostream>
#include <string.h>
#include <string>
#include <algorithm> 
#include <vector>
#include <chrono>
#include "../../lib/kclient.h"
// #include "kuksa.grpc.pb.h"
#include "../../lib/canbus.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <fstream>
#include <unordered_map> 
#include <linux/can.h>
#include <canMessage.h>
#include <KuksaItem.h>
#include <helper.h>
#include <unistd.h> 

using namespace std;
// using kuksa::AuthResponse;
// using kuksa::GetResponse;


#define CANADDRESS "vcan0"
#define SENSOR "sensor"
#define CANIDS "canIds"
#define PATH "path"
#define TYPE "Type"
#define IS2BYTE "Is it 2 byte"
#define BYTE1 "Byte 1"
#define BYTE2 "Byte 2"
#define BITSTARTBYTE1 "Bit byte 0 start"
#define BITENDBYTE1 "Bit byte 0 end"
#define BITSTARTBYTE2 "Bit byte 1 start"
#define BITENDBYTE2 "Bit byte 1 end"
#define MULTIPILICANT "Transform multiply"
#define SUMMATION "Transform sum"
#define CANID "canId"

#define  BASEPATH  "/opt/javaneh/auto_pilot_c/bin/"

class Can2kuksa
{
private:
    unordered_map<int, CanMessage*> canLists;
    vector<KuksaItem*> kuksaItems;
    vector<int> canIds;
    KukClient vehicle;
    Canbus canbus;
    vector<string> changedPath;
    vector<int> changedValue;
    uint64_t lastSend;
    Helper helper;
    void createCanFrame(string address);
    bool isExist(int id);

public:
    Can2kuksa(char **input);
    void run();
};


#endif
