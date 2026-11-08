#include "can2kuksa.h"



Can2kuksa::Can2kuksa(char **input)
{
    // helper = Helper();
    // lastSend = helper.timeSinceEpochMillisec();
    // vehicle = KukClient();
    // AuthResponse r = vehicle.authorize();
    canbus = Canbus(input[1]);
    cout << "Can bus has been created" << endl;
    string input_str(input[2]);
    string signalAddress = BASEPATH + input_str;
    cout << signalAddress << endl;
    createCanFrame(input_str);
}

void Can2kuksa::createCanFrame(string address)
{

    ifstream file(address);
    Json::Value actualJson;
    Json::Reader reader;
    reader.parse(file, actualJson);
    for (int i = 0; i < actualJson[SENSOR][CANIDS].size(); i++)
    {
        int canId = actualJson[SENSOR][CANIDS][i].asInt();
        canIds.push_back(canId);
        canLists[canId] = new CanMessage(canId);
    }
    for (int i = 0; i < actualJson[SENSOR][PATH].size(); i++)
    {
        string path = actualJson[SENSOR][PATH][i].asString();
        cout << path << endl;
        int id = actualJson[SENSOR][path][CANID].asInt();
        bool isIt2Byte = actualJson[SENSOR][path][IS2BYTE].asInt();
        int byte[2];
        byte[0] = actualJson[SENSOR][path][BYTE1].asInt();
        byte[1] = actualJson[SENSOR][path][BYTE2].asInt();
        int bitStart[2];
        bitStart[0] = actualJson[SENSOR][path][BITSTARTBYTE1].asInt();
        bitStart[1] = actualJson[SENSOR][path][BITSTARTBYTE2].asInt();
        int bitEnd[2];
        bitEnd[0] = actualJson[SENSOR][path][BITENDBYTE1].asInt();
        bitEnd[1] = actualJson[SENSOR][path][BITENDBYTE2].asInt();
        int multiplicant = actualJson[SENSOR][path][MULTIPILICANT].asInt();
        int summation = actualJson[SENSOR][path][SUMMATION].asInt();
	if(id == 19)
		cout << summation << endl;
        canLists[id]->addKuksaItem(new KuksaItem(path, isIt2Byte,  byte, bitStart, bitEnd, multiplicant, summation));
        kuksaItems.push_back(canLists[id]->items[canLists[id]->items.size()-1]);
    }
    cout << "End of Frame creation" << endl;
}


bool Can2kuksa::isExist(int id)
{
    for (int i = 0; i < canIds.size(); i++)
    {
        //cout << canIds[i] << endl;
        if (id == canIds[i])
        {
            return true;
        }
    }
    return false;
}


void Can2kuksa::run()
{
    while (1)
    {
        can_frame canFrame = canbus.readMessage();
        if (isExist(canFrame.can_id))
        {
            //cout << canFrame.can_id << endl;
            CanMessage* canCalled = canLists[canFrame.can_id];
            canCalled->updateValues(canFrame.data);
            
        }
        if (helper.timeSinceEpochMillisec() - lastSend > 50 )
        {
            lastSend = helper.timeSinceEpochMillisec();
            for (int i = 0; i < kuksaItems.size(); i++)
            {
                if (kuksaItems[i]->changedValue)
                {
                    kuksaItems[i]->changedValue = false;
                    changedPath.push_back(kuksaItems[i]->path);
                    changedValue.push_back(kuksaItems[i]->value);
                    //cout << kuksaItems[i]->path << "    :   " << kuksaItems[i]->value << endl;
                }
                
            }
            if (changedPath.size() > 0)
            {
                // vehicle.setPublish(changedPath,changedValue, changedPath.size());
                vehicle.set(changedPath,changedValue, changedPath.size());
            }
            
            changedPath.clear();
            changedValue.clear();
        }
        

        usleep(500);
    }
    
}
