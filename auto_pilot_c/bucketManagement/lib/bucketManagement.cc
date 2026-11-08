#include "bucketManagement.h"
string sensorPathsinit[26] = {
					// "Vehicle.Body.Load.BucketAngle.x",  // 0
					// "Vehicle.Body.Load.BucketAngle.y",  // 1
					// "Vehicle.Frame.Chassis.ChassisAngle.x",  // 2
					// "Vehicle.Frame.Chassis.ChassisAngle.y",  // 3
					"Vehicle.Energy.Hydrolic.OilPumpPressure",  // 4
					"Vehicle.Energy.Hydrolic.OilPumpTemp",  // 5
					"Vehicle.Energy.Hydrolic.OilTankLevel",  // 6
					"Vehicle.Body.Load.BucketWeight.YP.XP.0.weight",  // 7
					"Vehicle.Body.Load.BucketWeight.YP.XP.1.weight",  // 8
					"Vehicle.Body.Load.BucketWeight.YP.XP.2.weight",  // 9
					"Vehicle.Body.Load.BucketWeight.YP.XP.3.weight",  // 10
					"Vehicle.Body.Load.BucketWeight.YP.XN.0.weight",  // 11
					"Vehicle.Body.Load.BucketWeight.YP.XN.1.weight",  // 12
					"Vehicle.Body.Load.BucketWeight.YP.XN.2.weight",  // 13
					"Vehicle.Body.Load.BucketWeight.YP.XN.3.weight",  // 14
					"Vehicle.Body.Load.BucketWeight.YN.XP.0.weight",  // 15
					"Vehicle.Body.Load.BucketWeight.YN.XP.1.weight",  // 16	
					"Vehicle.Body.Load.BucketWeight.YN.XP.2.weight",  // 17
					"Vehicle.Body.Load.BucketWeight.YN.XP.3.weight",  // 18
					"Vehicle.Body.Load.BucketWeight.YN.XN.0.weight",  // 19
					"Vehicle.Body.Load.BucketWeight.YN.XN.1.weight",  // 20
					"Vehicle.Body.Load.BucketWeight.YN.XN.2.weight",  // 21
					"Vehicle.Body.Load.BucketWeight.YN.XN.3.weight",  // 22
					"Vehicle.Hardware.RemoteController.mode",  // 23
					"Vehicle.Hardware.RemoteController.LoadMode",  // 24
					"Vehicle.Hardware.RemoteController.ControlLoadMode", // 25
					"Vehicle.Hardware.RemoteController.Speed",  // 26
					"Vehicle.Hardware.RemoteController.SpeedDir",  // 27
					"Vehicle.Hardware.RemoteController.mission",  // 25
					"Vehicle.Hardware.RemoteController.missionDir"  // 25 // To Do
					// "Vehicle.Hardware.RemoteController.StAngle",  // 28 // To Do
					// "Vehicle.Hardware.RemoteController.StAngleDir"  // 29 // To Do

		};
		int initValue[26] = {110,0,70, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 1, 1, 1,0,1,0,1};
BucketManagement::BucketManagement()
{

    logLevel = 1;
    chassisAngleX = chassisAngleY = bucketAngleX = bucketAngleY = oilPumpP = oilPumpT = oilTankL = prev_sideUp = 0;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    bucketWeight[i][j][k] = 0;
                }
                
            }
            
        }

    // vehicle = KukClient();
    // AuthResponse r = vehicle.authorize();
    sideUp = 0;
    // int initValue[30] = {0};
    vehicle.set(sensorPathsinit, initValue, 26);
}

bool BucketManagement::loadData()
{
    vector<int> kukRespGet = vehicle.get(sensorPaths, 30);
    if(kukRespGet.size() == 30)
    {
        bucketAngleX = kukRespGet[0] * -1;
        bucketAngleY = kukRespGet[1] * -1;
        chassisAngleX = kukRespGet[2] * -1;
        chassisAngleY = kukRespGet[3] * -1;
        oilPumpP = kukRespGet[4];
        oilPumpT = kukRespGet[5];
        oilTankL = kukRespGet[6];
        int index = 6;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    index ++;
                    bucketWeight[i][j][k] = kukRespGet[index];
                }
                
            }
            
        }
        parkMode = kukRespGet[23];
        LoadMode = kukRespGet[24];
        ControllLoadMode = kukRespGet[25];
        int joy[4];
        for (int i = 0; i < 4; i++)
        {
            joy[i] = kukRespGet[26+i];
            ////cout << "Joy: " << i << " : " << joy[i] << "    ,   ";
        }
        //cout << endl;
        //cout << sideUp << " " << prev_sideUp << endl;
        if (joy[2] < 200 && joy[0] < 200 && joy[1] < 200)
        {
            sideUp = 0;
        }
        if (abs(bucketAngleX - chassisAngleX) < THRANGLEZERO + 1 && abs(bucketAngleY - chassisAngleY) < THRANGLEZERO + 1 && sideUp == 0)
        {
            prev_sideUp = 0;
        }
        if (joy[0] > 200 && joy[1] < 200 && joy[2] < 200 && (prev_sideUp == PSIDE || prev_sideUp == 0))
            {
                sideUp = PSIDE;
            prev_sideUp = PSIDE;
            }
            else if(joy[1] > 200 && joy[0] < 200 && joy[2] < 200 && (prev_sideUp == NSIDE || prev_sideUp == 0))
            {
                sideUp = NSIDE;
            prev_sideUp = NSIDE;
            }
            else if(joy[2] > 200 && joy[3] == 1 && joy[0] < 200 && joy[1] < 200 && (prev_sideUp == LSIDE || prev_sideUp == 0))
            {
                sideUp = LSIDE;
            prev_sideUp = LSIDE;
            }
            else if(joy[2] > 200 && joy[3] == 0 && joy[0] < 200 && joy[1] < 200 && (prev_sideUp == RSIDE || prev_sideUp == 0))
            {
                sideUp = RSIDE;
            prev_sideUp = RSIDE;
            }

        
        // //cout << sideUp << endl;
        return true;
    }
    else{
        //cout << "ERROR" << endl;
        return false;
    }

    
}
void printNewValue(int newValue, string name)
{
    //cout << "NEW VALUE OF " << name << " :  " << newValue << endl << "-----------------------------------" << endl << endl;
}
void setNewValueLogger(int &target, int value, string name)
{
    if(target != value)
    {
            target = value;
            printNewValue(target, name);
    }
}
bool BucketManagement::loadDataLogger()
{
    vector <int> kukRespGet = vehicle.get(sensorPaths, 30);
    if(kukRespGet.size() == 30)
    {
        setNewValueLogger(bucketAngleX, kukRespGet[0], "bucketAngleX");
        setNewValueLogger(bucketAngleY, kukRespGet[1], "bucketAngleY");
        setNewValueLogger(chassisAngleX, kukRespGet[2], "chassisAngleX");
        setNewValueLogger(chassisAngleY, kukRespGet[3], "chassisAngleY");
        setNewValueLogger(oilPumpP, kukRespGet[4], "oilPumpP");
        setNewValueLogger(oilPumpT, kukRespGet[5], "oilPumpT");
        setNewValueLogger(oilTankL, kukRespGet[6], "oilTankL");
        int index = 6;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    index ++;
                    string name = "bucketWeight[" + to_string(i) + "][" + to_string(j) + "][" + to_string(k) + "]";
                    setNewValueLogger(bucketWeight[i][j][k], kukRespGet[index], name);
                }
                
            }
            
        }
        setNewValueLogger(parkMode, kukRespGet[23], "parkMode");
        setNewValueLogger(LoadMode, kukRespGet[24], "LoadMode");
        setNewValueLogger(ControllLoadMode, kukRespGet[25], "ControllLoadMode");
        int joy[4];
        for (int i = 0; i < 4; i++)
        {
            joy[i] = kukRespGet[26+i];
        }
        if (joy[0] > 200 && joy[1] < 200 && joy[2] < 200)
        {
            setNewValueLogger(sideUp, PSIDE, "sideUp");
        }
        else if(joy[1] > 200 && joy[0] < 200 && joy[2] < 200)
        {
            setNewValueLogger(sideUp, NSIDE, "sideUp");
        }
        else if(joy[2] > 200 && joy[3] == 1 && joy[0] < 200 && joy[1] < 200)
        {
            setNewValueLogger(sideUp, LSIDE, "sideUp");
        }
        else if(joy[2] > 200 && joy[3] == 0 && joy[0] < 200 && joy[1] < 200)
        {
            setNewValueLogger(sideUp, RSIDE, "sideUp");
        }
        else
        {
            setNewValueLogger(sideUp, 0, "sideUp");
        }
        
        return true;
    }
    else{
        //cout << "ERROR" << endl;
        //cout << kukRespGet.size() << endl;
        return false;
    }

    
}

bool BucketManagement::isProperForUnloading()
{
    int minWeight = std::numeric_limits<int>::max();;
    int maxWeight = -1*std::numeric_limits<int>::max();;
    int diff =0;
    for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    int tmp = bucketWeight[i][j][k];
                    if (tmp > maxWeight)
                    {
                        maxWeight = tmp;
                    }
                    if (minWeight > tmp)
                    {
                        minWeight = tmp;
                    }
                }
                
            }
            
        }
    diff = maxWeight - minWeight;
    if (logLevel)
    {
        if (!(chassisAngleX < 5))
        {
            printNewValue(chassisAngleX < 5, "chassisAngleX < 5");
        }
        if (!(chassisAngleY < 5))
        {
            printNewValue(chassisAngleY < 5, "chassisAngleY < 5");
        }
        if (!(oilPumpP > 100))
        {
            printNewValue(oilPumpP > 100, "oilPumpP > 100");
        }
        if (!(oilPumpT < 50))
        {
            printNewValue(oilPumpT < 50, "oilPumpT < 50");
        }
        if (!(oilPumpT > -20))
        {
            printNewValue(oilPumpT > -20, "oilPumpT > -20");
        }
        if (!(oilTankL > 60))
        {
            printNewValue(oilTankL > 60, "oilTankL > 60");
        }
        if (!(maxWeight * 0.8 <= minWeight))
        {
            //cout << maxWeight << ": " << minWeight << endl;
            printNewValue(maxWeight * 0.8 < minWeight, "maxWeight * 0.8 < minWeight");
        }
    }
    return chassisAngleX < 5 && chassisAngleY < 5 && oilPumpP > 100 && oilPumpT < 50 && oilPumpT > -20 && oilTankL > 60  && maxWeight * 0.8 <= minWeight; // && diff < 20;
}
void BucketManagement::changeValveValue(int &value, string &address, int newValue)
{
    if(value != newValue)
    {
        value = newValue;
        pathChanged.push_back(address);
        valueChanged.push_back(value);
    }
}
void BucketManagement::setAllToZero(string hydrolicJacValve[2][2], int hydrolicJacValveValue[2][2])
{
    changeValveValue(hydrolicJacValveValue[YP][XP], hydrolicJacValve[YP][XP], 0);
    changeValveValue(hydrolicJacValveValue[YP][XN], hydrolicJacValve[YP][XN], 0);
    changeValveValue(hydrolicJacValveValue[YN][XP], hydrolicJacValve[YN][XP], 0);
    changeValveValue(hydrolicJacValveValue[YN][XN], hydrolicJacValve[YN][XN], 0);
}


void BucketManagement::raiseMode()
{
    setAllToZero(hydrolicJacValveClose, hydrolicJacValveCloseValue);
    changeValveValue(hydrolicMotorValue[1], hydrolicMotor[1], 0);
    //cout << prev_sideUp << " " << sideUp << endl;
    // cout << "Y: " << bucketAngleY << "       ,   X:  " << bucketAngleX << endl;
    // cout << "Y: " << chassisAngleY << "       ,   X:  " << chassisAngleX << endl;
    // cout << "Y: " << bucketAngleY - chassisAngleY << "       ,   X:  " << bucketAngleX - chassisAngleX << endl;
    // //cout << "sideUp:    " << sideUp << "    , prev_sideUp:    " << prev_sideUp << "    , diffrential angle X:    " << (bucketAngleX - chassisAngleX)  << "    , diffrential angle Y:    " << (bucketAngleX - chassisAngleX) << endl;
    // YP XPs
    
    
    if ((sideUp == LSIDE || (sideUp == RSIDE) ) &&   abs(bucketAngleX - chassisAngleX) > 20)
    {
        changeValveValue(flowControlValue, flowControl, 1);
    }
    
    if (((sideUp == PSIDE) && (bucketAngleX - chassisAngleX) < THRANGLESIDE) &&  abs(bucketAngleY - chassisAngleY) < 60 || ((sideUp == RSIDE) && (bucketAngleY - chassisAngleY) > -THRANGLESIDE && abs(bucketAngleX - chassisAngleX) < MAXANGLEX))
    {
        changeValveValue(hydrolicJacValveOpenValue[YP][XP], hydrolicJacValveOpen[YP][XP], 1);
    }
    else
    {
        changeValveValue(hydrolicJacValveOpenValue[YP][XP], hydrolicJacValveOpen[YP][XP], 0);
    }
    
    // YP XN
    if (((sideUp == PSIDE) && (bucketAngleX - chassisAngleX) >  -THRANGLESIDE) &&  abs(bucketAngleY - chassisAngleY) < 60 || ((sideUp == LSIDE) && (bucketAngleY - chassisAngleY) > -THRANGLESIDE && abs(bucketAngleX - chassisAngleX) < MAXANGLEX))
    {
        changeValveValue(hydrolicJacValveOpenValue[YP][XN], hydrolicJacValveOpen[YP][XN], 1);
    }
    else
    {
        changeValveValue(hydrolicJacValveOpenValue[YP][XN], hydrolicJacValveOpen[YP][XN], 0);
    }

    // YN XP
    if (((sideUp == NSIDE) && (bucketAngleX - chassisAngleX) < THRANGLESIDE) &&  abs(bucketAngleY - chassisAngleY) < 60 || ((sideUp == RSIDE) && (bucketAngleY - chassisAngleY) < THRANGLESIDE && abs(bucketAngleX - chassisAngleX) < MAXANGLEX))
    {
        changeValveValue(hydrolicJacValveOpenValue[YN][XP], hydrolicJacValveOpen[YN][XP], 1);
    }
    else
    {
        changeValveValue(hydrolicJacValveOpenValue[YN][XP], hydrolicJacValveOpen[YN][XP], 0);
    }


    // YN XN
    if (((sideUp == NSIDE) && (bucketAngleX - chassisAngleX) > -THRANGLESIDE) &&  abs(bucketAngleY - chassisAngleY) < 60 || ((sideUp == LSIDE) && (bucketAngleY - chassisAngleY) < THRANGLESIDE && abs(bucketAngleX - chassisAngleX) < MAXANGLEX ))
    {
        changeValveValue(hydrolicJacValveOpenValue[YN][XN], hydrolicJacValveOpen[YN][XN], 1);
    }
    else
    {
        changeValveValue(hydrolicJacValveOpenValue[YN][XN], hydrolicJacValveOpen[YN][XN], 0);
    }
    
}
void BucketManagement::backToNaturalPos()
{

    //cout << "Y: " << bucketAngleY << "       ,   X:  " << bucketAngleX << endl;
    changeValveValue(flowControlValue, flowControl, 0);
    if (abs(bucketAngleY - chassisAngleY) < 3 && abs(bucketAngleX - chassisAngleX) < 3)
    {
            changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 1);
    }
    else
    {
        setAllToZero(hydrolicJacValveOpen, hydrolicJacValveOpenValue);
        // YP XP
        // bool c1 = (prev_sideUp == PSIDE);
        // bool c2 = (bucketAngleX - chassisAngleX) < THRANGLEZERO;
        // bool c4 = (bucketAngleX - chassisAngleX) > -THRANGLEZERO;
        // bool c3 = abs(bucketAngleY - chassisAngleY) > THRANGLEZERO;
        //cout << "prev_sideUp == PSIDE" << c1 << endl;
        //cout << "(bucketAngleX - chassisAngleX) < THRANGLEZERO)" << c2 << endl;
        //cout << "abs(bucketAngleY - chassisAngleY) > THRANGLEZERO" << c3 << endl;
        //cout << "(bucketAngleX - chassisAngleX) > -THRANGLEZERO" << c4 << endl;

        if ((((prev_sideUp == PSIDE) || (prev_sideUp == NSIDE)) ))
        {
            if (abs(bucketAngleY - chassisAngleY) > 15)
            {
                changeValveValue(releafValue, releaf, 1);
            }
            // else if(abs(bucketAngleX - chassisAngleX) > 1)
            // {
            //     changeValveValue(releafValue, releaf, 0);
            // }
            else
            {
                changeValveValue(releafValue, releaf, 0);
            }
            
        }
        if ((((prev_sideUp == RSIDE) || (prev_sideUp == LSIDE)) ))
        {
            if (abs(bucketAngleX - chassisAngleX) > 10 )
            {
                changeValveValue(releafValue, releaf, 1);
            }
            // else if(abs(bucketAngleY - chassisAngleY) > 1 && abs(bucketAngleX - chassisAngleX) < 35)
            // {
            //     changeValveValue(releafValue, releaf, 0);
            // }
            else
            {
                changeValveValue(releafValue, releaf, 0);
            }
        }
        if (abs(bucketAngleX - chassisAngleX) < 1  && abs(bucketAngleY - chassisAngleY) < 1)
        {
                changeValveValue(releafValue, releaf, 1);
        }
        
        if (prev_sideUp == 0 && (abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
        {
            changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 1);
        }
        
        
        if (abs(bucketAngleX - chassisAngleX) > 0 || abs(bucketAngleY - chassisAngleY) > 10)
        {
            /* code */
            changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 1);
            // // if (((prev_sideUp == PSIDE) && (bucketAngleX - chassisAngleX) < THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == RSIDE) && (bucketAngleY - chassisAngleY) > -THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            // if ((prev_sideUp == PSIDE) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == RSIDE) && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            // {
            //     changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 1);
            // }
            // else
            // {
            //     changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 0);
            // }
            
            // // YP XN
            // // if (((prev_sideUp == PSIDE) && (bucketAngleX - chassisAngleX) > -THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == LSIDE) && (bucketAngleY - chassisAngleY) > -THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            // if ((prev_sideUp == PSIDE)  &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == LSIDE)  && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            // {
            //     changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 1);
            // }
            // else
            // {
            //     changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 0);
            // }

            // // YN XP
            // // if (((prev_sideUp == NSIDE) && (bucketAngleX - chassisAngleX) < THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == RSIDE) && (bucketAngleY - chassisAngleY) < THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            // if ((prev_sideUp == NSIDE) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == RSIDE) && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            // {
            //     changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 1);
            // }
            // else
            // {
            //     changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 0);
            // }


            // // YN XN
            // // if (((prev_sideUp == NSIDE) && (bucketAngleX - chassisAngleX) > -THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == LSIDE) && (bucketAngleY - chassisAngleY) < THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO ))
            // if ((prev_sideUp == NSIDE)  &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == LSIDE)  && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO ))
            // {
            //     changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 1);
            // }
            // else
            // {
            //     changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 0);
            // }

        }
        else
        {
            if (((prev_sideUp == PSIDE) && (bucketAngleX - chassisAngleX) < THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == RSIDE) && (bucketAngleY - chassisAngleY) > -THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 0);
            }
            
            // YP XN
            if (((prev_sideUp == PSIDE) && (bucketAngleX - chassisAngleX) > -THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == LSIDE) && (bucketAngleY - chassisAngleY) > -THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 0);
            }

            // YN XP
            if (((prev_sideUp == NSIDE) && (bucketAngleX - chassisAngleX) < THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == RSIDE) && (bucketAngleY - chassisAngleY) < THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 0);
            }


            // YN XN
            if (((prev_sideUp == NSIDE) && (bucketAngleX - chassisAngleX) > -THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == LSIDE) && (bucketAngleY - chassisAngleY) < THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO ))
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 0);
            }
        }

    }
    
    
}

void BucketManagement::ForcebackToNaturalPos()
{

    //cout << "Y: " << bucketAngleY << "       ,   X:  " << bucketAngleX << endl;
    changeValveValue(flowControlValue, flowControl, 0);
    if (abs(bucketAngleY - chassisAngleY) < 3 && abs(bucketAngleX - chassisAngleX) < 3)
    {
            changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 1);
    }
    else
    {
        setAllToZero(hydrolicJacValveOpen, hydrolicJacValveOpenValue);
        // YP XP
        bool c1 = (prev_sideUp == PSIDE);
        bool c2 = (bucketAngleX - chassisAngleX) < THRANGLEZERO;
        bool c4 = (bucketAngleX - chassisAngleX) > -THRANGLEZERO;
        bool c3 = abs(bucketAngleY - chassisAngleY) > THRANGLEZERO;
        //cout << "prev_sideUp == PSIDE" << c1 << endl;
        //cout << "(bucketAngleX - chassisAngleX) < THRANGLEZERO)" << c2 << endl;
        //cout << "abs(bucketAngleY - chassisAngleY) > THRANGLEZERO" << c3 << endl;
        //cout << "(bucketAngleX - chassisAngleX) > -THRANGLEZERO" << c4 << endl;

        if ((((prev_sideUp == PSIDE) || (prev_sideUp == NSIDE)) ))
        {
            if (abs(bucketAngleY - chassisAngleY) > 15)
            {
                changeValveValue(releafValue, releaf, 1);
            }
            // else if(abs(bucketAngleX - chassisAngleX) > 1)
            // {
            //     changeValveValue(releafValue, releaf, 0);
            // }
            else
            {
                changeValveValue(releafValue, releaf, 0);
            }
            
        }
        if ((((prev_sideUp == RSIDE) || (prev_sideUp == LSIDE)) ))
        {
            if (abs(bucketAngleX - chassisAngleX) > 10 )
            {
                changeValveValue(releafValue, releaf, 1);
            }
            // else if(abs(bucketAngleY - chassisAngleY) > 1 && abs(bucketAngleX - chassisAngleX) < 35)
            // {
            //     changeValveValue(releafValue, releaf, 0);
            // }
            else
            {
                changeValveValue(releafValue, releaf, 0);
            }
        }
        if (abs(bucketAngleX - chassisAngleX) < 1  && abs(bucketAngleY - chassisAngleY) < 1)
        {
                changeValveValue(releafValue, releaf, 1);
        }
        
        if (prev_sideUp == 0 && (abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
        {
            changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 1);
            changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 1);
        }
        
        
        if (abs(bucketAngleX - chassisAngleX) > 0 || abs(bucketAngleY - chassisAngleY) > 10)
        {
            /* code */
            // if (((prev_sideUp == PSIDE) && (bucketAngleX - chassisAngleX) < THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == RSIDE) && (bucketAngleY - chassisAngleY) > -THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            if (abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || (abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 0);
            }
            
            // YP XN
            // if (((prev_sideUp == PSIDE) && (bucketAngleX - chassisAngleX) > -THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == LSIDE) && (bucketAngleY - chassisAngleY) > -THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            if ( abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ( abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 0);
            }

            // YN XP
            // if (((prev_sideUp == NSIDE) && (bucketAngleX - chassisAngleX) < THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == RSIDE) && (bucketAngleY - chassisAngleY) < THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            if (abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || (abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 0);
            }


            // YN XN
            // if (((prev_sideUp == NSIDE) && (bucketAngleX - chassisAngleX) > -THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((prev_sideUp == LSIDE) && (bucketAngleY - chassisAngleY) < THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO ))
            if (abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || (abs(bucketAngleX - chassisAngleX) > THRANGLEZERO ))
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 0);
            }

        }
        else
        {
            if (((bucketAngleX - chassisAngleX) < THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((bucketAngleY - chassisAngleY) > -THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XP], hydrolicJacValveClose[YP][XP], 0);
            }
            
            // YP XN
            if (((bucketAngleX - chassisAngleX) > -THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ( (bucketAngleY - chassisAngleY) > -THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YP][XN], hydrolicJacValveClose[YP][XN], 0);
            }

            // YN XP
            if (((bucketAngleX - chassisAngleX) < THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((bucketAngleY - chassisAngleY) < THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO))
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XP], hydrolicJacValveClose[YN][XP], 0);
            }


            // YN XN
            if (( (bucketAngleX - chassisAngleX) > -THRANGLEZERO) &&  abs(bucketAngleY - chassisAngleY) > THRANGLEZERO || ((bucketAngleY - chassisAngleY) < THRANGLEZERO && abs(bucketAngleX - chassisAngleX) > THRANGLEZERO ))
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 1);
            }
            else
            {
                changeValveValue(hydrolicJacValveCloseValue[YN][XN], hydrolicJacValveClose[YN][XN], 0);
            }
        }

    }
    
    
}


void BucketManagement::controller()
{
    if (parkMode == PARK && isProperForUnloading())
    {
        if (LoadMode != STOP)
        {
            
            if (ControllLoadMode == RAISE)
            {
                // //cout << "raise" << endl;
                setAllToZero(hydrolicJacValveClose, hydrolicJacValveCloseValue);
                changeValveValue(hydrolicMotorValue[0], hydrolicMotor[0], HYDRLICMOTOR1ACTIVE);
                changeValveValue(hydrolicMotorValue[1], hydrolicMotor[1], HYDRLICMOTOR2ACTIVE);
                changeValveValue(releafValue, releaf, 1);

                raiseMode();
                return;
            }
            else if(ControllLoadMode == DECREASE)
            {
                //cout << "decrease" << endl;
                changeValveValue(hydrolicMotorValue[0], hydrolicMotor[0], HYDRLICMOTOR1ACTIVE);
                changeValveValue(hydrolicMotorValue[1], hydrolicMotor[1], 1);
                backToNaturalPos();
                return;
            }
            else
            {
                setAllToZero(hydrolicJacValveClose, hydrolicJacValveCloseValue);
                cout << "Stop Mode" << endl;
            }
            
        }
        else
        {
            changeValveValue(hydrolicMotorValue[0], hydrolicMotor[0], 0);
            changeValveValue(hydrolicMotorValue[1], hydrolicMotor[1], 0);
            //cout << "Change LoadMode!   " << LoadMode << endl;
        }
    }
    if (parkMode != PARK)
    {
        changeValveValue(hydrolicMotorValue[0], hydrolicMotor[0], 0);
        changeValveValue(hydrolicMotorValue[1], hydrolicMotor[1], 0);
        //cout << "You are not in Park Mode" << endl << endl;
        changeValveValue(releafValue, releaf, 0);
        ForcebackToNaturalPos();
    }
    
    setAllToZero(hydrolicJacValveOpen, hydrolicJacValveOpenValue);
    // if (LoadMode != STOP)
    // {
    // }

    
    // //cout << "You can't unload" << endl;
    
}

void BucketManagement::run()
{
    while (1)
    {
        if(loadData())
        {
            controller();
            if (pathChanged.size() > 0)
            {
                if (logLevel)
                    for (int i = 0; i < pathChanged.size(); i++)
                        //cout << pathChanged[i] << ": " << valueChanged[i] << endl;
                vehicle.set(pathChanged, valueChanged, pathChanged.size());
            }
            pathChanged.clear();
            valueChanged.clear();
        }
        usleep(500);
    }
    
}
