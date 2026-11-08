#include "controller.h"

controllerRationalSteering::controllerRationalSteering(/* args */)
{
    isItturnInPlace = isSafeMode = 0;
    trackIn = 0.27;
    trackOut = 0.27;
    wheelBaseIn = 0.11;
    wheelBaseOut = 0.32;
    tireDia = 0.1;
    radiusMin = 0.35;
    angle = 0;
    vehicleSpeed = 2;
    radiusMultiplicant[0] = 1;
    radiusMultiplicant[1] = 1;
    raduis = radiusMin;
    wheelRadius[0] = radiusMin;
    wheelRadius[1] = radiusMin;
    isItturnInPlace = 0;
    deg = 0;
    rpm[0] = 0;
    rpm[1] = 0;
    turnDirection = 0;
    wheelDirection = 1;
    stop = 0;
    isSafeMode = 1;
    autoPilotMode = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            motorValue[i][j] = 0;
        }
    }
    brakeC = 0;
    vehicle = KukClient();
    AuthResponse raduis = vehicle.authorize();
    


    autoPilotModePath.push_back("Vehicle.Hardware.RemoteController.parkMode");
    autoPilotModePath.push_back("Vehicle.Hardware.RemoteController.isTurningPlace");
    autoPilotModePath.push_back("Vehicle.Hardware.RemoteController.ControlLoadMode");
    autoPilotModePath.push_back("Vehicle.Hardware.RemoteController.BrakeCommand");


    radioPath.push_back("Vehicle.Hardware.RemoteController.Speed");
    radioPath.push_back("Vehicle.Hardware.RemoteController.SpeedDir");
    radioPath.push_back("Vehicle.Hardware.RemoteController.StAngle");
    radioPath.push_back("Vehicle.Hardware.RemoteController.StAngleDir");
    radioPath.push_back("Vehicle.Hardware.RemoteController.isSafeMode");
    radioPath.push_back("Vehicle.Hardware.RemoteController.isTurningPlace");
    radioPath.push_back("Vehicle.Hardware.FrontSide");

    autoPilotPath.push_back("Vehicle.Software.AutoPilot.Speed");
    autoPilotPath.push_back("Vehicle.Software.AutoPilot.SpeedDir");
    autoPilotPath.push_back("Vehicle.Software.AutoPilot.StAngle");
    autoPilotPath.push_back("Vehicle.Software.AutoPilot.StAngleDir");
    autoPilotPath.push_back("Vehicle.Software.AutoPilot.isSafeMode");
    autoPilotPath.push_back("Vehicle.Software.AutoPilot.isTurningPlace");
    autoPilotPath.push_back("Vehicle.Software.FrontSide");
    

    motorPath[0][0] = "Vehicle.Corner.YP.XP.Motor.Inner.Speed";
    motorPath[0][1] = "Vehicle.Corner.YP.XP.Motor.Inner.Direction";
    motorPath[7][0] = "Vehicle.Corner.YP.XP.Motor.Outer.Speed";
    motorPath[7][1] = "Vehicle.Corner.YP.XP.Motor.Outer.Direction";

    motorPath[2][0] = "Vehicle.Corner.YN.XP.Motor.Inner.Speed";
    motorPath[2][1] = "Vehicle.Corner.YN.XP.Motor.Inner.Direction";
    motorPath[3][0] = "Vehicle.Corner.YN.XP.Motor.Outer.Speed";
    motorPath[3][1] = "Vehicle.Corner.YN.XP.Motor.Outer.Direction";

    motorPath[4][0] = "Vehicle.Corner.YP.XN.Motor.Inner.Speed";
    motorPath[4][1] = "Vehicle.Corner.YP.XN.Motor.Inner.Direction";
    motorPath[5][0] = "Vehicle.Corner.YP.XN.Motor.Outer.Speed";
    motorPath[5][1] = "Vehicle.Corner.YP.XN.Motor.Outer.Direction";

    motorPath[1][0] = "Vehicle.Corner.YN.XN.Motor.Inner.Speed";
    motorPath[1][1] = "Vehicle.Corner.YN.XN.Motor.Inner.Direction";
    motorPath[6][0] = "Vehicle.Corner.YN.XN.Motor.Outer.Speed";
    motorPath[6][1] = "Vehicle.Corner.YN.XN.Motor.Outer.Direction";
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            changedPath.push_back(motorPath[i][j]);
            changedValue.push_back(0);
        }
    }      
    vehicle.set(changedPath, changedValue, 16);
    changedPath.clear();
    changedValue.clear();
}

controllerRationalSteering::~controllerRationalSteering()
{
}

void controllerRationalSteering::radiusCalculation()
{
    if (deg < 0.02)
    {
        raduis = 10000;
    }
    else
    {
        raduis = radiusMin * 10 * (1 - deg) / 2 + radiusMin;
    }
}

void controllerRationalSteering::wheelRadiusCalculation()
{
    if (raduis > 500)
	{
	    wheelRadius[MOTOR_R] = raduis;
        wheelRadius[MOTOR_L] = raduis;
	}
    else
    {
        double a = pow(wheelBaseOut / 2, 2);
        double b = pow(raduis + trackIn / 2, 2);
        double c = pow(raduis - trackIn / 2, 2);
        double outerValue = sqrt(a+b);
        double innerValue = sqrt(a+c);
        if (turnDirection)
        {
            wheelRadius[MOTOR_L] = outerValue;
            wheelRadius[MOTOR_R] = innerValue; 
        }
        else
        {
            wheelRadius[MOTOR_R] = outerValue;
            wheelRadius[MOTOR_L] = innerValue; 
        }
    }
    
}

double controllerRationalSteering::overFlowRpm()
{
    double speedMaxWheel = MAX_RPM * 3.6 * M_PI / 60 * tireDia;
    double a = pow(wheelBaseOut / 2, 2);
    double b = pow(raduis + trackIn / 2, 2);
    double c = pow(raduis - trackIn / 2, 2);
    double outerValue = sqrt(a+b);
    double innerValue = sqrt(a+c);
    double rwheel = outerValue;
    double vcar = speedMaxWheel * raduis / rwheel;
    rwheel = innerValue;
    double out = vcar / raduis * rwheel / 3.6 * 60 / M_PI / tireDia;
    return out;
}

void controllerRationalSteering::radiusMultiplicantCallculator()
{
    radiusMultiplicant[MOTOR_R] = wheelRadius[MOTOR_R] /  raduis;
	radiusMultiplicant[MOTOR_L] = wheelRadius[MOTOR_L] /  raduis;
	rpmAverage = (radiusMultiplicant[MOTOR_R]+radiusMultiplicant[MOTOR_L]) / 2;
	if (radiusMultiplicant[MOTOR_R] > radiusMultiplicant[MOTOR_L])
    {	
        radiusMultiplicant[MOTOR_R] = radiusMultiplicant[MOTOR_R] * OUTERHIGHMULTIPLICANT;
        radiusMultiplicant[MOTOR_L] = radiusMultiplicant[MOTOR_L] * INNERHIGHMULTIPLICANT;
    }
    if (radiusMultiplicant[MOTOR_L] > radiusMultiplicant[MOTOR_R])
    {
        radiusMultiplicant[MOTOR_L] = radiusMultiplicant[MOTOR_L] * OUTERHIGHMULTIPLICANT;
        radiusMultiplicant[MOTOR_R] = radiusMultiplicant[MOTOR_R] * INNERHIGHMULTIPLICANT;
    }
	if (vehicleSpeed < 1)
	{
		if (radiusMultiplicant[MOTOR_R] > radiusMultiplicant[MOTOR_L])
		{
			radiusMultiplicant[MOTOR_R] = radiusMultiplicant[MOTOR_R] * OUTERMULTIPLICANT;
			radiusMultiplicant[MOTOR_L] = radiusMultiplicant[MOTOR_L] * INNERMULTIPLICANT;
		}
		else if (radiusMultiplicant[MOTOR_L] > radiusMultiplicant[MOTOR_R])
		{
			radiusMultiplicant[MOTOR_L] = radiusMultiplicant[MOTOR_L] * OUTERMULTIPLICANT;
			radiusMultiplicant[MOTOR_R] = radiusMultiplicant[MOTOR_R] * INNERMULTIPLICANT;
		}
	}
}

void controllerRationalSteering::rpmCalculator()
{

    double speed[2];

    radiusMultiplicantCallculator();
    speed[MOTOR_R] = vehicleSpeed * radiusMultiplicant[MOTOR_R];
    speed[MOTOR_L] = vehicleSpeed * radiusMultiplicant[MOTOR_L];

    rpmAverage = rpmAverage * vehicleSpeed / 3.6 * 60 / M_PI / tireDia;

    float newRpmAverage = (rpm[MOTOR_R] + rpm[MOTOR_L]) / 2;
    float newToOld = newRpmAverage / rpmAverage;

    rpm[MOTOR_R] = rpm[MOTOR_R] - newRpmAverage * (newToOld - 1);
    rpm[MOTOR_L] = rpm[MOTOR_L] - newRpmAverage * (newToOld - 1);

    float newRpmAverage2 = (rpm[MOTOR_R] + rpm[MOTOR_L]) / 2;

    rpm[MOTOR_R] =  speed[0]/ 3.6 * 60 / M_PI / tireDia;
    rpm[MOTOR_L] = speed[1] / 3.6 * 60 / M_PI / tireDia;
    if (rpm[MOTOR_R] > MAX_RPM && rpm[MOTOR_L] > MAX_RPM)
    {
        rpm[MOTOR_R] = MAX_RPM;
        rpm[MOTOR_L] = MAX_RPM;
    }
    else if (rpm[MOTOR_R] > MAX_RPM)
    {
        rpm[MOTOR_R] = MAX_RPM;
        rpm[MOTOR_L] = overFlowRpm();
    }
    else if (rpm[MOTOR_L] > MAX_RPM)
    {
        rpm[MOTOR_L] = MAX_RPM;
        rpm[MOTOR_R] = overFlowRpm();
    }

    if (vehicleSpeed < MINIMUM_SPEED_FAST_TURN)
    {
        if (deg > 0.97)
        {
            if (turnDirection)
            {
                rpm[MOTOR_R] = 0;
            }
            else
            {
                rpm[MOTOR_L] = 0;
            }
        }
    }
}

void controllerRationalSteering::setVehicleSpeed(double speed, int direction)
{
    vehicleSpeed = speed;
    if (isSafeMode)
        vehicleSpeed = speed / 2;
    if (frontSide == STOP)
    {
        stop = 1;
        wheelDirection = 0;
        vehicleSpeed = 0;
    }
    else if (frontSide == P)
    {
        stop = 0;
        wheelDirection = direction;
    }
    else
    {
        stop = 0;
        if (direction)
        {
            wheelDirection = 0;
        }
        else
        {
            wheelDirection = 1;
        }
        
    }
    // cout << "vehicleSpeed:  " << vehicleSpeed << "  wheelDirection:  " << wheelDirection << endl;
    
}

void controllerRationalSteering::setAngle(double angle2, int direction)
{
    if (frontSide == STOP)
    {
        stop = 1;
        turnDirection = 0;
    }
    else if (frontSide == P)
    {
        stop = 0;
        turnDirection = direction;
    }
    else
    {
        stop = 0;
        if (direction)
        {
            turnDirection = 0;
        }
        else
        {
            turnDirection = 1;
        }
        
    }
    deg = angle2 / 100;
    radiusCalculation();
    wheelRadiusCalculation();
}

void controllerRationalSteering::motorValueCalculation()
{
    if (stop == 1)
    {
        setChangedValue(MOTOR_R, SPEED, 0);
        setChangedValue(MOTOR_L, SPEED, 0);
    }
    else
    {
        if (wheelDirection)
        {
            setChangedValue(MOTOR_R, DIRECTION, 1);
            setChangedValue(MOTOR_L, DIRECTION, 0);
        }
        else
        {
            setChangedValue(MOTOR_R, DIRECTION, 0);
            setChangedValue(MOTOR_L, DIRECTION, 1);
        }
        setChangedValue(MOTOR_R, SPEED, rpm[MOTOR_R] * 255 / 120);
        setChangedValue(MOTOR_L, SPEED, rpm[MOTOR_L] * 255 / 120);
    }
}

void controllerRationalSteering::forceStop(int state)
{
    stop = state;
}

void controllerRationalSteering::speedCalculation()
{
    if (brakeC > 0)
    {
        setVehicleSpeed(0,0);
        stop = 1;
    }
    else
    {
        stop = 0;
    }
    
    if(isItturnInPlace == 0)
    {
        rpmCalculator();
        motorValueCalculation();
    }
    else
    {
        turnInPlace();
    }
}

void controllerRationalSteering::turnInPlace()
{
    if (stop == 1)
    {
        setChangedValue(MOTOR_R, SPEED, 0);
        setChangedValue(MOTOR_L, SPEED, 0);
    }
    else
    {
        if (deg < 0.05)
        {    
            
            rpmCalculator();
            motorValueCalculation();
        }
        else
        {
            if(isSafeMode == 1)
            {
                setChangedValue(MOTOR_R, SPEED, vehicleSpeed * 200);
                setChangedValue(MOTOR_L, SPEED, vehicleSpeed * 200);
            }
            if (turnDirection)
            {
                if (wheelDirection)
                {
                    setChangedValue(MOTOR_L, DIRECTION, 0);
                    setChangedValue(MOTOR_R, DIRECTION, 0);
                }
                else
                {
                    setChangedValue(MOTOR_L, DIRECTION, 1);
                    setChangedValue(MOTOR_R, DIRECTION, 1);
                }
            }
            else
            {
                if (wheelDirection)
                {
                    setChangedValue(MOTOR_L, DIRECTION, 1);
                    setChangedValue(MOTOR_R, DIRECTION, 1);
                }
                else
                {
                    setChangedValue(MOTOR_L, DIRECTION, 0);
                    setChangedValue(MOTOR_R, DIRECTION, 0);
                }
            }
        }
    }
}

void controllerRationalSteering::turningInPlaceState(int state)
{
    isItturnInPlace = state;
}

void controllerRationalSteering::safeMode(int state)
{
    isSafeMode = state;
}

void controllerRationalSteering::setChangedValue(int motorID, int type, int value)
{ 
    if (motorValue[motorID][type] != value)
    {
        motorValue[motorID][type] = value;
        changedPath.push_back(motorPath[motorID][type]);
        changedValue.push_back(motorValue[motorID][type]);
    }
    
}
void controllerRationalSteering::sendData()
{
    if (changedPath.size() > 0)
    {
        for (int i = 0; i < changedPath.size(); i++)
        {
            cout << changedPath[i] << " :   " << changedValue[i] << endl;
        }
        cout << endl << endl;        
        vehicle.set(changedPath, changedValue, changedValue.size());
    }
    changedPath.clear();
    changedValue.clear();
    
}

void controllerRationalSteering::loadData()
 {
    GetResponse kuksaGetResponse = vehicle.get(autoPilotModePath,autoPilotModePath.size());
    if(vehicle.connection_state == 2 && kuksaGetResponse.values_size() == autoPilotModePath.size())
    {
        autoPilotMode = kuksaGetResponse.values(2).valueint32();
        int start = kuksaGetResponse.values(1).valueint32();
        int mode = kuksaGetResponse.values(0).valueint32();
        brakeC = kuksaGetResponse.values(3).valueint32();
        if (mode == STOP)
        {
            stop = 1;
            setVehicleSpeed(0,0);
            setAngle(0, 0);
        }
        else
        {
            if (autoPilotMode == 1 || (autoPilotMode != 1 && start != STOP))
            {
                int speedDirection, steeringAngleDirection, safeModeState, turnInPlace;
                double speedInput, steeringAngle;
                if (autoPilotMode == AUTOPILOTMODE)
                {
                    kuksaGetResponse = vehicle.get(autoPilotPath, autoPilotPath.size());
                }
                else
                {
                    kuksaGetResponse = vehicle.get(radioPath, radioPath.size());
                }
                if(vehicle.connection_state == 2 && kuksaGetResponse.values_size() == autoPilotPath.size())
                {
                    speedInput = kuksaGetResponse.values(0).valueint32() * 1.8 / 255;
                    speedDirection = kuksaGetResponse.values(1).valueint32();
                    steeringAngle = kuksaGetResponse.values(2).valueint32();
                    steeringAngleDirection = kuksaGetResponse.values(3).valueint32();
                    safeModeState = kuksaGetResponse.values(4).valueint32();
                    turnInPlace = kuksaGetResponse.values(5).valueint32();
                    steeringAngle = steeringAngle / 255 * 100;
                    frontSide = kuksaGetResponse.values(6).valueint32();
                    setVehicleSpeed(speedInput,speedDirection);
                    setAngle(steeringAngle, steeringAngleDirection);
                    if (autoPilotMode == 1)
                    {
                        turningInPlaceState(turnInPlace);
                    }
                    else
                    {
                        turningInPlaceState(0);
                    }
                    safeMode(safeModeState);
                }
                else
                {
                    for (int i = 0; i < kuksaGetResponse.values_size(); i++)
                    {
                        cout << kuksaGetResponse.values(i).path() << endl;
                    }
                    
                }
            }
            else
            {
                stop = 1;
                setVehicleSpeed(0,0);
                setAngle(0, 0);
            }
        }
    }
    else
    {
        for (int i = 0; i < kuksaGetResponse.values_size(); i++)
        {
            cout << kuksaGetResponse.values(i).path() << endl;
        }
        
    }
 }





void controllerRationalSteering::run()
{
    setVehicleSpeed(0,0);

    while (true)
    {

        try{
            loadData();
            speedCalculation();
            sendData();
            usleep(50000);
        }
        catch(exception& e)
        {
            cout << "exeption happening" << endl;
        }
        
    }
    
}
