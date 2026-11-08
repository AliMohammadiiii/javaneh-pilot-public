#include "kuk2can.h"
#include <math.h>
kuk2can::kuk2can(char **input)
{
    helper = Helper();
    lastSend = helper.timeSinceEpochMillisec();
    canbus = new Canbus(input[1]);
    cout << "Can bus has been created" << endl;
    string input_str(input[2]);
    string signalAddress = BASEPATH + input_str;
    cout << signalAddress << endl;
    number_of_actuator = 0;
    // AuthResponse r = vehicle.authorize();
    dataCanStructsCreator(signalAddress);
    // vector <int> defualtvalue;
    // for (int i = 0; i < paths.size(); i++)
    // {
    //     defualtvalue.push_back(0);
    //     cout << paths[i] << endl;
    // }
    // vehicle.set(paths, defualtvalue, paths.size());
}

kuk2can::~kuk2can()
{



}


CanMessage* findCan(vector<CanMessage> &canMessages, int id)
{
    for (int i = 0; i < canMessages.size(); i++)
    {
        if (canMessages[i].id == id)
        {
            return &canMessages[i];
        }
    }
    
}

int* kuk2can::findCanAddress(vector<CanMessage> &canMessages, int id, int byte)
{
    CanMessage *tmp = findCan(canMessages, id);
    // //cout << "id:    " << id << "    Byte:   " << byte << "  address " << &(tmp->data[byte]) << endl;
    return &(tmp->data[byte]);
    
}

void kuk2can::createCanMessages(int id)
{
    CanMessage tmp;
    tmp.id = id;
    int tmpdata[8] = {0,0,0,0,0,0,0,0};
    copy_n(tmpdata, 8, tmp.data);
    canMessages.push_back(tmp);
}

void kuk2can::dataCanStructsCreator(string signalAddress)
{

    ifstream file(signalAddress);
    Json::Value actualJson;
    Json::Reader reader;
    reader.parse(file, actualJson);
    for (int i = 0; i < actualJson[ACTUATOR][CANIDS].size(); i++)
    {
        createCanMessages(actualJson[ACTUATOR][CANIDS][i].asInt());
    }

    //cout << "size Can messge" << actualJson[ACTUATOR][CANIDS].size() << endl;
    for (int i = 0; i < actualJson[ACTUATOR][PATH].size(); i++)
    {
        string path = actualJson[ACTUATOR][PATH][i].asString();
        paths.push_back(path);
        DataCanStruct *tmpdatacanstruct = new DataCanStruct;
        tmpdatacanstruct->bitstart = actualJson[ACTUATOR][path][BITSTARTBYTE1].asInt();
        // tmpdatacanstruct->size = actualJson[ACTUATOR][path]["size"].asInt();
        tmpdatacanstruct->size = 1;
        if (tmpdatacanstruct->size > 1)
        {
            tmpdatacanstruct->type = "int8";
        }
        else
        {
            tmpdatacanstruct->type = "bool";
        }
        
        // tmpdatacanstruct->type = actualJson[ACTUATOR][path][TYPE].asString();
        int id = actualJson[ACTUATOR][path][CANID].asInt();
        int byte = actualJson[ACTUATOR][path][BYTE1].asInt();
        tmpdatacanstruct->canAddress = findCanAddress(canMessages,id,byte);
        dataCanStructs.push_back(*tmpdatacanstruct);
        number_of_actuator++;
    }
}


void kuk2can::loadData()
{   
    vector<int> kukRespGet = vehicle.get(paths, paths.size());
    for (int i = 0; i < canMessages.size(); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            canMessages[i].data[j] = 0;
        }
        
    }
    
    if(kukRespGet.size() == paths.size())
    {
        for (int i = 0; i < number_of_actuator; i++)
        {
            if (dataCanStructs[i].type == "int8")
            {
                int *add = dataCanStructs[i].canAddress;
                *add = kukRespGet[i];
            }
            else if (dataCanStructs[i].type == "bool")
            {
                int *add = dataCanStructs[i].canAddress;
                *add += kukRespGet[i] * (pow(2,dataCanStructs[i].bitstart));
            }
        }
    }
    else
    {
        //cout << "ERROR" << endl;
    }
}
void kuk2can::sendMessage()
{
    for(int i =0; i < canMessages.size(); i++)
    {   
        canbus->send(canMessages[i].id, canMessages[i].data);
    }
}
void kuk2can::run()
{
    while (true)
    {
        loadData();
        sendMessage();
        usleep(10000);
    }
    
}
