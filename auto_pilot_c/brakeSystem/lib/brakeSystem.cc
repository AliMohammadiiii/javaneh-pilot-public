#include "brakeSystem.h"



brakeSystem::brakeSystem()
{
    // vehicle = KukClient();
    // AuthResponse r = vehicle.authorize();
    parkC = PARK;
    brakeC = 0;
    setBrakesPath();
    int initValue[NUMBEROFSENSOR] = {0};
    // vehicle.set(path, initValue, NUMBEROFSENSOR);
    // vehicle.set(rotationSpeed, initValue, 8);
    isLoggingMode = false;
    prevNumberOfParkBrake = 0;
    prevNumberOfServiceBrake = 0;
    string pathPark[8]
    {
        PATH_PARKBRAKE_OUTER_YPXP,
        PATH_PARKBRAKE_INNER_YPXP,
        PATH_PARKBRAKE_OUTER_YPXN,
        PATH_PARKBRAKE_INNER_YPXN,
        PATH_PARKBRAKE_OUTER_YNXP,
        PATH_PARKBRAKE_INNER_YNXP,
        PATH_PARKBRAKE_OUTER_YNXN,
        PATH_PARKBRAKE_INNER_YNXN
    };
    int initValuePark[8] = {1};
    // vehicle.set(pathPark, initValuePark, 8);
    string pathBrake[8]
    {
        PATH_BRAKEBRAKE_OUTER_YPXP,
        PATH_BRAKEBRAKE_INNER_YPXP,
        PATH_BRAKEBRAKE_OUTER_YPXN,
        PATH_BRAKEBRAKE_INNER_YPXN,
        PATH_BRAKEBRAKE_OUTER_YNXP,
        PATH_BRAKEBRAKE_INNER_YNXP,
        PATH_BRAKEBRAKE_OUTER_YNXN,
        PATH_BRAKEBRAKE_INNER_YNXN
    };
    // vehicle.set(pathPark, initValue, 8);
}

void brakeSystem::setBrakesPath()
{
    brakeRows[YPOUTER] = new BrakeRow();
    brakeRows[YPOUTER]->id = YPOUTER;
    brakeRows[YPOUTER]->parkBrakes[RIGHT]->setPath(PATH_PARKBRAKE_OUTER_YPXP);
    brakeRows[YPOUTER]->parkBrakes[LEFT]->setPath(PATH_PARKBRAKE_OUTER_YPXN);
    
    brakeRows[YPOUTER]->serviceBrakes[RIGHT]->setPath(PATH_BRAKEBRAKE_OUTER_YPXP);
    brakeRows[YPOUTER]->serviceBrakes[LEFT]->setPath(PATH_BRAKEBRAKE_OUTER_YPXN);

    brakeRows[YPOUTER]->releaseBrakes[RIGHT]->setPath(PATH_RELEASEBRAKE_OUTER_YPXP);
    brakeRows[YPOUTER]->releaseBrakes[LEFT]->setPath(PATH_RELEASEBRAKE_OUTER_YPXN);

    brakeRows[YPINNER] = new BrakeRow();
    brakeRows[YPINNER]->id = YPINNER;
    brakeRows[YPINNER]->parkBrakes[RIGHT]->setPath(PATH_PARKBRAKE_INNER_YPXP);
    brakeRows[YPINNER]->parkBrakes[LEFT]->setPath(PATH_PARKBRAKE_INNER_YPXN);
    
    brakeRows[YPINNER]->serviceBrakes[RIGHT]->setPath(PATH_BRAKEBRAKE_INNER_YPXP);
    brakeRows[YPINNER]->serviceBrakes[LEFT]->setPath(PATH_BRAKEBRAKE_INNER_YPXN);

    brakeRows[YPINNER]->releaseBrakes[RIGHT]->setPath(PATH_RELEASEBRAKE_INNER_YPXP);
    brakeRows[YPINNER]->releaseBrakes[LEFT]->setPath(PATH_RELEASEBRAKE_INNER_YPXN);

    brakeRows[YNOUTER] = new BrakeRow();
    brakeRows[YNOUTER]->id = YNOUTER;
    brakeRows[YNOUTER]->parkBrakes[RIGHT]->setPath(PATH_PARKBRAKE_OUTER_YNXP);
    brakeRows[YNOUTER]->parkBrakes[LEFT]->setPath(PATH_PARKBRAKE_OUTER_YNXN);
    
    brakeRows[YNOUTER]->serviceBrakes[RIGHT]->setPath(PATH_BRAKEBRAKE_OUTER_YNXP);
    brakeRows[YNOUTER]->serviceBrakes[LEFT]->setPath(PATH_BRAKEBRAKE_OUTER_YNXN);

    brakeRows[YNOUTER]->releaseBrakes[RIGHT]->setPath(PATH_RELEASEBRAKE_OUTER_YNXP);
    brakeRows[YNOUTER]->releaseBrakes[LEFT]->setPath(PATH_RELEASEBRAKE_OUTER_YNXN);

    brakeRows[YNINNER] = new BrakeRow();
    brakeRows[YNINNER]->id = YNINNER;
    brakeRows[YNINNER]->parkBrakes[RIGHT]->setPath(PATH_PARKBRAKE_INNER_YNXP);
    brakeRows[YNINNER]->parkBrakes[LEFT]->setPath(PATH_PARKBRAKE_INNER_YNXN);
    
    brakeRows[YNINNER]->serviceBrakes[RIGHT]->setPath(PATH_BRAKEBRAKE_INNER_YNXP);
    brakeRows[YNINNER]->serviceBrakes[LEFT]->setPath(PATH_BRAKEBRAKE_INNER_YNXN);

    brakeRows[YNINNER]->releaseBrakes[RIGHT]->setPath(PATH_RELEASEBRAKE_INNER_YNXP);
    brakeRows[YNINNER]->releaseBrakes[LEFT]->setPath(PATH_RELEASEBRAKE_INNER_YNXN);
}


void brakeSystem::roundBrakeC()
{
    int remaining = brakeC % 10;
    brakeC -= remaining;
    if (remaining > 2.5 && remaining < 7.5)
    {
        brakeC += 5;
    }
    else if (remaining > 7.5)
    {
        brakeC += 10;
    }
}

BrakeRow* brakeSystem::findbrakeRow(int id)
{
    for (int i = 0; i < NUMBEROFROWS; i++)
    {
        if (brakeRows[i]->id == id)
        {
            return brakeRows[i];
        }
    }
    return nullptr;
}
bool brakeSystem::loadData()
{
 
    vector<int> kukRespGet = vehicle.get(path, NUMBEROFSENSOR);
    if(kukRespGet.size() == NUMBEROFSENSOR)
    {
           if (isLoggingMode)
    {
        //printf("Load Data\n");
        //cout << "-----------------------------------------------------" << endl << endl;
    }
        findbrakeRow(YPOUTER)->setTemp(kukRespGet[0], kukRespGet[1]);
        findbrakeRow(YPINNER)->setTemp(kukRespGet[2], kukRespGet[3]);
        findbrakeRow(YNOUTER)->setTemp(kukRespGet[4], kukRespGet[5]);
        findbrakeRow(YNINNER)->setTemp(kukRespGet[6], kukRespGet[7]);
        parkC = kukRespGet[8];
        brakeC = kukRespGet[9] * 100 / 255;
        roundBrakeC();
        if (isLoggingMode)
        {
            //printf("Value of Brake command is: %d, %d\n",brakeC, kukRespGet[9]);
            //cout << "-----------------------------------------------------" << endl << endl;
        }
        for (int i = 0; i < 8; i++)
        {
            rotationSpeedValue[i] = kukRespGet[10 + i];
        }
        if (isLoggingMode)
        {
            for (int i = 0; i < 8; i++)
            {
                //printf("Wheel Rotation Value is: %d\n",rotationSpeedValue[i]);
            }
            //cout<< "-----------------------------------------------------" << endl << endl;
        }
        return true;
    }
    else{
        //cout<< "ERROR" << endl;
        //cout<< kukRespGet.size() << endl;
        
        
        usleep(100000000);
        return false;
    }

}

void brakeSystem::sendData()
{
    //cout << changedPath.size() << endl;
    if (isLoggingMode)
    {
        // //printf("Send Data\n");
        // //cout << "-----------------------------------------------------" << endl << endl;
    }
    if (changedPath.size() > 0)
    {
        if (isLoggingMode)
        {
            for (int i = 0; i < changedPath.size(); i++)
            {
                //printf("Update VAlue of %s with %d\n",changedPath[i], changedValue[i]);
            }
            //cout << "-----------------------------------------------------" << endl << endl;
        }
        
        vehicle.set(changedPath, changedValue, changedPath.size());
        changedPath.clear();
        changedValue.clear();
    }
    
}


void brakeSystem::run()
{
    while (1)
    {
        if (loadData())
        {
            calculator();
            sendData();
            usleep(1000);
        }
    }
    
}

void brakeSystem::calcNumberOfBrakes()
{
    if (parkC == PARK)
    {
        numberOfParkBrake = NUMBEROFROWS;
        numberOfServiceBrake = 0;
    }
    else
    {
        int tempBrakeC = brakeC;
        // //cout<< brakeC << endl;
        numberOfServiceBrake = 0;
            while (tempBrakeC - 20 >= 0)
            {
                tempBrakeC -= 20;
                numberOfServiceBrake += 1;
            }
            if(numberOfServiceBrake > 4)
            {
                numberOfServiceBrake = 4;
                tempBrakeC += 20;
            }
            numberOfParkBrake = 0;
            while (tempBrakeC - 5 >= 0)
            {
                tempBrakeC -= 5;
                numberOfParkBrake += 1;
            }

        int numberOfAvailableRow = isOverHeat();
        if ( numberOfAvailableRow < numberOfServiceBrake || numberOfAvailableRow == 0)
        {
            if (isMoving())
            {
                if (isLoggingMode)
                {
                    //printf("Is Over heated and in Move\n");
                    //cout<< "-----------------------------------------------------" << endl << endl;
                }
                numberOfServiceBrake = NUMBEROFROWS;
                numberOfParkBrake = 0;
            }
            else
            {
                if (isLoggingMode)
                {
                    //printf("Is Over heated and it stay\n");
                    //cout<< "-----------------------------------------------------" << endl << endl;
                }
                numberOfParkBrake = NUMBEROFROWS;
                numberOfServiceBrake = 0;
            }
            
        }
    }
        if (isLoggingMode)
        {
            //printf("We need %d Park Brake and %d Serivce brake for %d percentage Brake\n", numberOfParkBrake, numberOfServiceBrake, brakeC);
            //cout<< "-----------------------------------------------------" << endl << endl;
        }
    
    
}
int brakeSystem::isOverHeat()
{
    int flag = NUMBEROFROWS;
    for (int i = 0; i < NUMBEROFROWS; i++)
    {
        if (!(brakeRows[i]->temp[RIGHT] < 200 && brakeRows[i]->temp[LEFT] < 200))
        {
            flag -= 1;
            if (isLoggingMode)
            {
                //printf("Brake of row %d isn't in normal Temp range with value of Right: %d , Left: %d\n",brakeRows[i]->id, brakeRows[i]->temp[RIGHT], brakeRows[i]->temp[LEFT]);
                //cout<< "-----------------------------------------------------" << endl << endl;
            }
        }
    }
    if (isLoggingMode)
    {
        //printf("Number of overheated %d\n",flag);
        //cout<< "-----------------------------------------------------" << endl << endl;
    }
    return flag;
}

bool brakeSystem::isMoving()
{
    for (int i = 0; i < 8; i++)
    {
    if (rotationSpeedValue[i] > 10)
    {

        return true;
    }
                
    }
    return false;

}

void brakeSystem::selectBrakePark()
{

    while (numberOfParkBrake < prevNumberOfParkBrake)
    {
        if (isLoggingMode)
        {
            //printf("numberOfParkBrake : %d < prevNumberOfParkBrake: %d\n", numberOfParkBrake, prevNumberOfParkBrake);
            //cout<< "-----------------------------------------------------" << endl << endl;
        }
        
        for (int i = NUMBEROFROWS -1; i >= 0; i--)
        {

            if(brakeRows[i]->parkBrakes[RIGHT]->isUsed && brakeRows[i]->parkBrakes[LEFT]->isUsed)
            {
                brakeRows[i]->parkBrakes[RIGHT]->deactive();
                brakeRows[i]->parkBrakes[LEFT]->deactive();
                changedPath.push_back(brakeRows[i]->parkBrakes[RIGHT]->path);
                changedValue.push_back(brakeRows[i]->parkBrakes[RIGHT]->value);
                changedPath.push_back(brakeRows[i]->parkBrakes[LEFT]->path);
                changedValue.push_back(brakeRows[i]->parkBrakes[LEFT]->value);
                prevNumberOfParkBrake --;
                if (isLoggingMode)
                {
                    //printf("Disable new Service Brake row %d\n", i);
                    //cout<< "-----------------------------------------------------" << endl << endl;
                }
            }
            if (numberOfParkBrake == prevNumberOfParkBrake)
            {
                break;
            }

        }
    }
    while (numberOfParkBrake > prevNumberOfParkBrake)
    {
        if (isLoggingMode)
        {
            //printf("numberOfParkBrake : %d > prevNumberOfParkBrake: %d\n", numberOfParkBrake, prevNumberOfParkBrake);
            //cout<< "-----------------------------------------------------" << endl << endl;
        }
        for (int i = 0; i < NUMBEROFROWS; i++)
        {
            if(!brakeRows[i]->parkBrakes[RIGHT]->isUsed && !brakeRows[i]->parkBrakes[LEFT]->isUsed)
            {
                brakeRows[i]->parkBrakes[RIGHT]->active();
                brakeRows[i]->parkBrakes[LEFT]->active();
                changedPath.push_back(brakeRows[i]->parkBrakes[RIGHT]->path);
                changedValue.push_back(brakeRows[i]->parkBrakes[RIGHT]->value);
                changedPath.push_back(brakeRows[i]->parkBrakes[LEFT]->path);
                changedValue.push_back(brakeRows[i]->parkBrakes[LEFT]->value);
                prevNumberOfParkBrake++;
                if (isLoggingMode)
                {
                    //cout<< prevNumberOfParkBrake << endl;
                    //printf("Active new Service Brake row %d\n", i);
                    //cout<< "-----------------------------------------------------" << endl << endl;
                }
            }
            if (numberOfParkBrake == prevNumberOfParkBrake)
            {
                break;
            }
        }
    }   
}


void brakeSystem::selectBrakeService()
{
    while (numberOfServiceBrake < prevNumberOfServiceBrake)
    {
        if (isLoggingMode)
        {
            //printf("numberOfServiceBrake : %d < prevNumberOfServiceBrake: %d\n", numberOfServiceBrake, prevNumberOfServiceBrake);
            //cout<< "-----------------------------------------------------" << endl << endl;
        }
        for (int i = NUMBEROFROWS -1; i >= 0; i--)
        {
            if(brakeRows[i]->serviceBrakes[RIGHT]->isUsed && brakeRows[i]->serviceBrakes[LEFT]->isUsed)
            {
                brakeRows[i]->serviceBrakes[RIGHT]->deactive();
                brakeRows[i]->serviceBrakes[LEFT]->deactive();
                changedPath.push_back(brakeRows[i]->serviceBrakes[RIGHT]->path);
                changedValue.push_back(brakeRows[i]->serviceBrakes[RIGHT]->value);
                changedPath.push_back(brakeRows[i]->serviceBrakes[LEFT]->path);
                changedValue.push_back(brakeRows[i]->serviceBrakes[LEFT]->value);
                
                brakeRows[i]->releaseBrakes[RIGHT]->deactive();
                brakeRows[i]->releaseBrakes[LEFT]->deactive();
                changedPath.push_back(brakeRows[i]->releaseBrakes[RIGHT]->path);
                changedValue.push_back(brakeRows[i]->releaseBrakes[RIGHT]->value);
                changedPath.push_back(brakeRows[i]->releaseBrakes[LEFT]->path);
                changedValue.push_back(brakeRows[i]->releaseBrakes[LEFT]->value);


                prevNumberOfServiceBrake--;
                if (isLoggingMode)
                {
                    //printf("Disable new Park Brake row %d\n", i);
                    //cout<< "-----------------------------------------------------" << endl << endl;
                }
            }
            if (numberOfServiceBrake == prevNumberOfServiceBrake)
            {
                break;
            }
        }
    }

    while(numberOfServiceBrake > prevNumberOfServiceBrake)
    {
        if (isLoggingMode)
        {
            //printf("numberOfServiceBrake : %d > prevNumberOfServiceBrake: %d\n", numberOfServiceBrake, prevNumberOfServiceBrake);
            //cout<< "-----------------------------------------------------" << endl << endl;
        }
        for (int index = 0; index < 4; index++)
        {
            if(!brakeRows[index]->serviceBrakes[RIGHT]->isUsed && !brakeRows[index]->serviceBrakes[LEFT]->isUsed)
            {
                brakeRows[index]->serviceBrakes[RIGHT]->active();
                brakeRows[index]->serviceBrakes[LEFT]->active();
                brakeRows[index]->numberOfActive++;
                changedPath.push_back(brakeRows[index]->serviceBrakes[RIGHT]->path);
                changedValue.push_back(brakeRows[index]->serviceBrakes[RIGHT]->value);
                changedPath.push_back(brakeRows[index]->serviceBrakes[LEFT]->path);
                changedValue.push_back(brakeRows[index]->serviceBrakes[LEFT]->value);

                brakeRows[index]->releaseBrakes[RIGHT]->active();
                brakeRows[index]->releaseBrakes[LEFT]->active();
                changedPath.push_back(brakeRows[index]->releaseBrakes[RIGHT]->path);
                changedValue.push_back(brakeRows[index]->releaseBrakes[RIGHT]->value);
                changedPath.push_back(brakeRows[index]->releaseBrakes[LEFT]->path);
                changedValue.push_back(brakeRows[index]->releaseBrakes[LEFT]->value);
                prevNumberOfServiceBrake++;
                if (isLoggingMode)
                {
                    //printf("Active new Park Brake row %d\n", index);
                    //cout<< "-----------------------------------------------------" << endl << endl;
                }
            }
            if (numberOfServiceBrake == prevNumberOfServiceBrake)
            {
                break;
            }
            
        }
    }   
}

bool comparePtrToNode(BrakeRow* a, BrakeRow* b) {
    if (a->totalTemp == b->totalTemp)
    {
        return a->numberOfActive < b->numberOfActive;
    }
    
     return (a->totalTemp < b->totalTemp); 
}
void brakeSystem::calculator()
{
    if (isLoggingMode)
    {
        //printf("Calculation\n");
        //cout << "-----------------------------------------------------" << endl << endl;
    }
    calcNumberOfBrakes();

    sort(brakeRows, brakeRows+4, comparePtrToNode);
    

    if (isLoggingMode)
    {
        for (int i = 0; i < NUMBEROFROWS; i++)
        {
            //printf("Temp of %d : %d and Number of active: %d\n", brakeRows[i]->id, brakeRows[i]->totalTemp, brakeRows[i]->numberOfActive);
        }
        //cout << "-----------------------------------------------------" << endl << endl;
        for (int i = 0; i < NUMBEROFROWS; i++)
        {
            //printf("State of %d row Right Park : %d Brake : %d  ,   Left Park : %d Brake : %d\n", brakeRows[i]->id, brakeRows[i]->parkBrakes[RIGHT]->isUsed, brakeRows[i]->serviceBrakes[RIGHT]->isUsed, brakeRows[i]->parkBrakes[LEFT]->isUsed, brakeRows[i]->serviceBrakes[LEFT]->isUsed);
        }
        //cout << "-----------------------------------------------------" << endl << endl;
    }   

    selectBrakePark();

    selectBrakeService();
    
}
