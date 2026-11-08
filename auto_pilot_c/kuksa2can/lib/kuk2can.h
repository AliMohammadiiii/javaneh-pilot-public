#include <iostream>
#include <string.h>
#include <string>
#include <algorithm> 
#include <vector>
#include <kclient.h>

// #include "kuksa.grpc.pb.h"
#include <canbus.h>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <fstream>
#include <helper.h>
using namespace std;


#define CANADDRESS "vcan0"
#define ACTUATOR "actuator"
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



// using kuksa::AuthResponse;
// using kuksa::GetResponse;
struct dataCanStruct
{
    int* canAddress;
    int size;
    int bitstart;
    string type;
} typedef DataCanStruct;

struct canMessage
{
    int id;
    int data[8];
    

} typedef CanMessage;


class kuk2can
{
private:
    vector<string> paths;
    vector<DataCanStruct> dataCanStructs;
    vector<CanMessage> canMessages;
    int number_of_actuator;
    KukClient vehicle;
    Helper helper;
    Canbus *canbus;
    uint64_t lastSend;
    void loadData();
    void sendMessage();
    void canMessagesSetup();
    void dataCanStructsCreator(string signalAddress);
    void createCanMessages(int id);
    int* findCanAddress(vector<CanMessage> &canMessages, int id, int byte);

public:
    kuk2can(char **input);
    ~kuk2can();
    void run();
};



