#include "utilityControl.h"


utilityControl::utilityControl()
{
    vehicle = KukClient();
    AuthResponse r = vehicle.authorize();

}
void utilityControl::run()
{
    while (1)
    {
        if (loadData())
        {
            calculator();
            sendData();
            usleep(5000);
        }
    }
}


bool utilityControl::loadData()
{
    GetResponse kukRespGet = vehicle.get(sensorPath, NUMBEROFSENSOR);
    if(vehicle.connection_state == 2 && kukRespGet.values_size() == NUMBEROFSENSOR)
    {
        
        frontSide = kukRespGet.values(0).valueint32();
        return true;
    }
    else{
        cout << "ERROR" << endl;
        cout << kukRespGet.values_size() << endl;
        for (int i = 0; i < kukRespGet.values_size(); i++)
        {
            cout << kukRespGet.values(i).path() << endl;
        }
        
        usleep(100000000);
        return false;
    }

}

void utilityControl::calculator()
{
    if (frontSide == YPFRONTSIDE)
    {
        for (int i = 0; i < 2; i++)
        {
            if (headLight[YP][i] != 1)
            {
                changedPath.push_back(actuatorPath[YP + i]);
                changedValue.push_back(1);
                headLight[YP][i] = 1;
            }
            if (headLight[YN][i] != 0)
            {
                changedPath.push_back(actuatorPath[YN + i + 1]);
                changedValue.push_back(0);
                headLight[YN][i] = 0;
            }
        }
        
    }
    else if (frontSide == YNFRONTSIDE)
    {
        for (int i = 0; i < 2; i++)
        {
            if (headLight[YP][i] != 0)
            {
                changedPath.push_back(actuatorPath[YP + i]);
                changedValue.push_back(0);
                headLight[YP][i] = 0;
            }
            if (headLight[YN][i] != 1)
            {
                changedPath.push_back(actuatorPath[YN + i + 1]);
                changedValue.push_back(1);
                headLight[YN][i] = 1;
            }
        }
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            if (headLight[YP][i] != 0)
            {
                changedPath.push_back(actuatorPath[YP + i]);
                changedValue.push_back(0);
                headLight[YP][i] = 0;
            }
            if (headLight[YN][i] != 0)
            {
                changedPath.push_back(actuatorPath[YN + i + 1]);
                changedValue.push_back(0);
                headLight[YN][i] = 0;
            }
        }
    }
}

void utilityControl::sendData()
{
    if (changedPath.size() > 0)
    {
        if (isLoggingMode)
        {
            for (int i = 0; i < changedPath.size(); i++)
            {
                printf("Update VAlue of %s with %d\n",changedPath[i], changedValue[i]);
            }
            cout << "-----------------------------------------------------" << endl << endl;
        }
        
        vehicle.set(changedPath, changedValue, changedPath.size());
        changedPath.clear();
        changedValue.clear();
    }
    
}
