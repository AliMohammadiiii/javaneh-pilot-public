
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <time.h>
#include <math.h>


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
long long canSendTime = 0;

long long millis(){
    struct timespec _t;
    clock_gettime(CLOCK_REALTIME, &_t);
    return _t.tv_sec*1000 + lround(_t.tv_nsec/1e6);
}


void sendCan(int data[4], int id)
{
	  if ( millis() - canSendTime < 5)
		  return;
	  canSendTime = millis();
	  int s;
	  struct sockaddr_can addr;
	  struct ifreq ifr;
	  struct can_frame frame;

	  if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	  {
	    perror("Socket");
	    return;
	  }

	  strcpy(ifr.ifr_name, "can0");
	  ioctl(s, SIOCGIFINDEX, &ifr);

	  memset(&addr, 0, sizeof(addr));
	  addr.can_family = AF_CAN;
	  addr.can_ifindex = ifr.ifr_ifindex;

	  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	  {
	    perror("Bind");
	    return;
	  }

	  frame.can_id = id;
	  frame.can_dlc = 5;
	  frame.data[0] = data[0];
	  frame.data[1] = data[1];
	  frame.data[2] = data[2];
	  frame.data[3] = data[3];
	  //printf("%d: %d: %d %d %d\n",frame.can_id, frame.data[0], frame.data[1], frame.data[2], frame.data[3]);
	  if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
	  {
	    perror("Write");
	    return;
	  }

	  if (close(s) < 0)
	  {
	    perror("Close");
	    return;
	  }
	}
	void readCan(int filterId, int *value)
	{
	  int s, i;
	  int nbytes;
	  struct sockaddr_can addr;
	  struct ifreq ifr;
	  struct can_frame frame;

	  if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	  {
	    perror("Socket");
	    return;
	  }

	  strcpy(ifr.ifr_name, "can0");
	  ioctl(s, SIOCGIFINDEX, &ifr);

	  memset(&addr, 0, sizeof(addr));
	  addr.can_family = AF_CAN;
	  addr.can_ifindex = ifr.ifr_ifindex;

	  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	  {
	    perror("Bind");
	    return;
	  }
	  if (filterId != -1)
	  {
	    struct can_filter rfilter[1];

	    rfilter[0].can_id = filterId;
	    rfilter[0].can_mask = 0x7ff;

	    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
	  }
	  nbytes = read(s, &frame, sizeof(struct can_frame));

	  if (nbytes < 0)
	  {
	    perror("Read");
	    return;
	  }
	  value[0] = frame.can_id;
	  for (i = 0; i < 4; i++)
	    value[i + 1] = frame.data[i];

	  if (close(s) < 0)
	  {
	    perror("Close");
	    return;
	  }
	}

	struct RationalSteering
	{
	  double trackIn, trackOut, wheelBaseIn, wheelBaseOut, tireDia, rMin;
	  double rpmAverage,angle, v, r, rWheel[2], rpm[2], deg, turnDirection, wheelDirection, stop, rs[2];
	  struct
	  {
	    double speed, direction;
	  } motorValue[2];
	  int isItturnInPlace, isSafeMode;
	};

	void rCalc(struct RationalSteering *s)
	{
	  if (s->deg < 0.02)
	  {
	    s->r = 10000;
	  }
	  else
	  {
	    s->r = s->rMin * 10 * (1 - s->deg) / 2 + s->rMin;
	  }
	 // s->r = s->r * s->v / 2 + 0.12;
	//  if (s->r > 0 && s->r < 100)
  //	printf("r: %f\n", s->r);
}

// Calculate the radius of the wheels based on the robot's radius and the steering direction.
void rWheelCalc(struct RationalSteering *s)
{
	if (s->r > 500)
	{
		s->rWheel[MOTOR_R] = s->r;
    s->rWheel[MOTOR_L] = s->r;
    return;
	}

  if (s->turnDirection)
  {
	  double a = pow(s->wheelBaseOut / 2, 2);
	  double b = pow(s->r + s->trackIn / 2, 2);
	  double c = pow(s->r - s->trackIn / 2, 2);
	  double e = sqrt(a+b);
	  double d = sqrt(a+c);
	  //printf("%f %f %f %f %f\n",a, b, c, e, d);
    s->rWheel[MOTOR_L] = e;
    s->rWheel[MOTOR_R] = d; 
  }
  else
  {
	  double a = pow(s->wheelBaseOut / 2, 2);
	  double b = pow(s->r + s->trackIn / 2, 2);
	  double c = pow(s->r - s->trackIn / 2, 2);
	  double e = sqrt(a+b);
	  double d = sqrt(a+c);
	  //printf("%f %f %f %f %f\n",a, b, c, e, d);
    s->rWheel[MOTOR_R] = e;
    s->rWheel[MOTOR_L] = d; 
  }
//printf("rWheel :%f %f\n",s->rWheel[MOTOR_L], s->rWheel[MOTOR_R]);
}
double overFlowRpm(struct RationalSteering *s)
{
  double vmaxWheel = MAX_RPM * 3.6 * M_PI / 60 * s->tireDia;
	  double a = pow(s->wheelBaseOut / 2, 2);
	  double b = pow(s->r + s->trackIn / 2, 2);
	  double c = pow(s->r - s->trackIn / 2, 2);
	  double e = sqrt(a+b);
	  double d = sqrt(a+c);
  double rwheel = e;
  double vcar = vmaxWheel * s->r / rwheel;
  rwheel = d;
  return vcar / s->r * rwheel / 3.6 * 60 / M_PI / s->tireDia;
}

// eslah zarib v
void rsCalc(struct RationalSteering *s)
{
	s->rs[MOTOR_R] = s->rWheel[MOTOR_R] /  s->r;
	s->rs[MOTOR_L] = s->rWheel[MOTOR_L] /  s->r;
	s->rpmAverage = (s->rs[MOTOR_R]+s->rs[MOTOR_L])/2;
	if (s->rs[MOTOR_R] > s->rs[MOTOR_L])
                {	
                        s->rs[MOTOR_R] = s->rs[MOTOR_R] * 1.4;
                        s->rs[MOTOR_L] = s->rs[MOTOR_L] * 0.6;
                }
                if (s->rs[MOTOR_L] > s->rs[MOTOR_R])
                {
                        s->rs[MOTOR_L] = s->rs[MOTOR_L] * 1.4;
                        s->rs[MOTOR_R] = s->rs[MOTOR_R] * 0.6;
                }
	if (s->v < 1)
	{
		//printf("%f %f\n", s->rs[MOTOR_R], s->rs[MOTOR_L]);
		if (s->rs[MOTOR_R] > s->rs[MOTOR_L])
		{
			s->rs[MOTOR_R] = s->rs[MOTOR_R] * 1.2;
			s->rs[MOTOR_L] = s->rs[MOTOR_L] * 0.8;
		}
		else if (s->rs[MOTOR_L] > s->rs[MOTOR_R])
		{
			s->rs[MOTOR_L] = s->rs[MOTOR_L] * 1.2;
			s->rs[MOTOR_R] = s->rs[MOTOR_R] * 0.8;
		}

	}
	return;

}




// Calculate the RPM of the wheels based on the robot's speed and radius.
void rpmCacl(struct RationalSteering *s)
{
  double v[2];
  rsCalc(s);
  //v[0] = s->v / s->r * s->rWheel[MOTOR_R];
  //v[1] = s->v / s->r * s->rWheel[MOTOR_L];
  v[0] = s->v * s->rs[MOTOR_R];
  v[1] = s->v * s->rs[MOTOR_L];
  //printf("V wheel: %f %f\n", v[0], v[1]);
  s->rpmAverage = s->rpmAverage * s->v / 3.6 * 60 / M_PI / s->tireDia;
	float newRpmAverage = (s->rpm[MOTOR_R] + s->rpm[MOTOR_L]) / 2;
	float newToOld = newRpmAverage / s->rpmAverage;
	s->rpm[MOTOR_R] = s->rpm[MOTOR_R] - newRpmAverage * (newToOld - 1);
	s->rpm[MOTOR_L] = s->rpm[MOTOR_L] - newRpmAverage * (newToOld - 1);
	float newRpmAverage2 = (s->rpm[MOTOR_R] + s->rpm[MOTOR_L]) / 2;
  s->rpm[MOTOR_R] =  v[0]/ 3.6 * 60 / M_PI / s->tireDia;
  s->rpm[MOTOR_L] = v[1] / 3.6 * 60 / M_PI / s->tireDia;
  if (s->rpm[MOTOR_R] > MAX_RPM && s->rpm[MOTOR_L] > MAX_RPM)
  {
    s->rpm[MOTOR_R] = MAX_RPM;
    s->rpm[MOTOR_L] = MAX_RPM;
  }
  else if (s->rpm[MOTOR_R] > MAX_RPM)
  {
    s->rpm[MOTOR_R] = MAX_RPM;
    s->rpm[MOTOR_L] = overFlowRpm(s);
  }
    else if (s->rpm[MOTOR_L] > MAX_RPM)
  {
    s->rpm[MOTOR_L] = MAX_RPM;
    s->rpm[MOTOR_R] = overFlowRpm(s);
  }
 
   // printf("rpm: %f %f\n", s->rpm[MOTOR_L], s->rpm[MOTOR_R]);
	//printf("%f , %f , %f, %f , %f, %f\n",s->v , s->deg, newRpmAverage,newRpmAverage2, s->rpmAverage, newRpmAverage / s->rpmAverage);
 
	if (s->v < MINIMUM_SPEED_FAST_TURN)
  {
	  if (s->deg > 0.98)
	  {
		if (s->turnDirection)
		{
			s->rpm[MOTOR_R] = 0;
//			s->rpm[MOTOR_L] *= 1.5;
		}
		else
		{
			s->rpm[MOTOR_L] = 0;
//			s->rpm[MOTOR_R] *= 1.5;
		}
	  }
  }
 if(s->rpm[MOTOR_L] > 100 || s->rpm[MOTOR_R] > 100)
        printf("%f , %f , %f, %f , %f, %f\n",s->v , s->deg, newRpmAverage,newRpmAverage2, s->rpmAverage, newRpmAverage / s->rpmAverage);

//printf("alertttttttt, %f, %f \n", s->rpm[MOTOR_R],s->rpm[MOTOR_L]); 
    //printf("rpm: %f %f\n", s->rpm[MOTOR_L], s->rpm[MOTOR_R]);
}



void setV(struct RationalSteering *s, double v, int direction)
{
  s->v = v;
  if (s->isSafeMode)
	s->v = s->v / 2;
  s->wheelDirection = direction;
  //printf("%f\n",s->v);
}

void setAngle(struct RationalSteering *s, double angle, int direction)
{
  s->turnDirection = direction;
  s->deg = angle / 100;
  rCalc(s);
  rWheelCalc(s);

}

void motorCal(struct RationalSteering *s)
{
	if (s->stop == 1)
	{
		s->motorValue[MOTOR_R].speed = 0;
    		s->motorValue[MOTOR_L].speed= 0;
		return;
	}
  if (s->stop && s->wheelDirection)
  {
    s->motorValue[MOTOR_L].speed = 0;
    s->motorValue[MOTOR_R].speed = 0;
  }
  else
  {
    if (s->wheelDirection)
    {
      s->motorValue[MOTOR_L].direction = 0;
      s->motorValue[MOTOR_R].direction = 1;
    }
    else
    {
      s->motorValue[MOTOR_L].direction = 1;
      s->motorValue[MOTOR_R].direction = 0;
    
    }
  }
    s->motorValue[MOTOR_R].speed = s->rpm[MOTOR_R] * 255 / 120;
    s->motorValue[MOTOR_L].speed = s->rpm[MOTOR_L] * 255 / 120;
 // printf("motoro value speed: %f %f\n", s->motorValue[MOTOR_L].speed, s->motorValue[MOTOR_R].speed);
}
void forceStop(struct RationalSteering *s,int state)
{
  s->stop = state;
}
void motorCanSend(struct RationalSteering *s)
{
  int data[4];
  data[0] = s->motorValue[MOTOR_R].speed;
  data[1] = s->motorValue[MOTOR_R].direction;
  data[2] = s->motorValue[MOTOR_L].speed;
  data[3] = s->motorValue[MOTOR_L].direction;
  sendCan(data,ID_CAN_MOTOR);
}
void turnInPlace(struct RationalSteering *s)
{
	s->motorValue[MOTOR_R].speed = s->v * 100;
    s->motorValue[MOTOR_L].speed = s->v * 100;
    if (s->deg < 0.05)
    {
	    
    if (s->wheelDirection)
    {
      s->motorValue[MOTOR_L].direction = 0;
      s->motorValue[MOTOR_R].direction = 1;
    }
    else
    {
      s->motorValue[MOTOR_L].direction = 1;
      s->motorValue[MOTOR_R].direction = 0;
    }
    return;
    }

    if(s->isSafeMode == 1)
    {
	    s->motorValue[MOTOR_L].speed = s->motorValue[MOTOR_L].speed * 2;
	    s->motorValue[MOTOR_R].speed = s->motorValue[MOTOR_R].speed * 2;
    }
    if (s->turnDirection)
    {
	    if (s->wheelDirection)
	    {
	      s->motorValue[MOTOR_L].direction = 0;
	      s->motorValue[MOTOR_R].direction = 0;
	    }
	    else
	    {
	      s->motorValue[MOTOR_L].direction = 1;
	      s->motorValue[MOTOR_R].direction = 1;
	    }
    }
    else
    {
	    if (s->wheelDirection)
	    {
	      s->motorValue[MOTOR_L].direction = 1;
	      s->motorValue[MOTOR_R].direction = 1;
	    }
	    else
	    {
	      s->motorValue[MOTOR_L].direction = 0;
	      s->motorValue[MOTOR_R].direction = 0;
	    }
    }
}
void sendSpeed(struct RationalSteering *s)
{
	
	if(s->isItturnInPlace == 0)
	{
		rpmCacl(s);
		motorCal(s);
	}
	else
	{
		turnInPlace(s);
	}
	
	//printf("%d\n",s->isItturnInPlace);
	//rpmCacl(s);
        //        motorCal(s);
  motorCanSend(s);
}
void ultrasonicAlert(struct RationalSteering *s, int* data)
{
	//printf("%d\n",data[0]);
	if(data[1] < MINIMUM_DISTANCE || data[1] < MINIMUM_DISTANCE)
		forceStop(s,1);
	else
		forceStop(s,0);
}
void turningInPlaceState(struct RationalSteering *s, int* data)
{
	s->isItturnInPlace = data[1];
}
void changeMovement(struct RationalSteering *s, int* data)
{
  switch (data[1])
  {
  case ID_CHANGE_SPEED:
	double speed = data[2] * 1.8;
	speed = speed / 255;
//	printf("v: %d %f\n", data[2], speed);
    setV(s, speed, data[3]);
    break;
  case ID_CHANGE_ANGLE:
    setAngle(s, data[2], data[3]);
    break;
  case ID_BREAK:
    setV(s, 0, 0);
    break;
  default:
    break;
  }
}
void safeMode(struct RationalSteering *s, int* data)
{
	if (data[1] == 0)
	{
		s->isSafeMode = 1;
	}
	else
	{
		s->isSafeMode = 0;
	}
}
int main()
{
  canSendTime = millis();
  struct RationalSteering s;
  s.trackIn = 0.27;
  s.trackOut = 0.27;
  s.wheelBaseIn = 0.11;
  s.wheelBaseOut = 0.32;
  s.tireDia = 0.1;
  s.rMin = 0.35;
  s.angle = 0;
  s.v = 2;
  s.rs[0] = 1;
  s.rs[1] = 1;
  s.r = s.rMin;
  s.rWheel[0] = s.rMin;
  s.rWheel[1] = s.rMin;
  s.isItturnInPlace = 0;
  s.deg = 0;
  s.rpm[0] = 0;
  s.rpm[1] = 0;
  s.turnDirection = 0;
  s.wheelDirection = 1;
  s.stop = 0;
  s.isSafeMode = 1;
  setV(&s,0,0);
  while (1)
  {
    sendSpeed(&s);
    int data[5];
    readCan(-1,data);
    if (data[0] == ID_CAN_MOVEMENT)
    {
	    //printf("%d: %d %d %d\n", data[1], data[2], data[3], data[4]);
      changeMovement(&s, data);
    }
	else if(data[0] == ID_ULTRASONIC)
	{
	//	ultrasonicAlert(&s,data);
	}
	else if(data[0] == 0x032)
	{
		turningInPlaceState(&s,data);
	}

	else if(data[0] == 0x033)
	{
		safeMode(&s,data);
	}
  }
  return 0;
}
