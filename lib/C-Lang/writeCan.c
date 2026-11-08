
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <math.h>
void sendCan(int data[4], int id)
{
	int s;
        struct sockaddr_can addr;
        struct ifreq ifr;
        struct can_frame frame;

        if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
                perror("Socket");
                return;
        }

        strcpy(ifr.ifr_name, "can0" );
        ioctl(s, SIOCGIFINDEX, &ifr);

        memset(&addr, 0, sizeof(addr));
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("Bind");
                return;
        }

        frame.can_id = id;
        frame.can_dlc = 5;
	frame.data[0] = data[0];
	frame.data[1] = data[1];
	frame.data[2] = data[2];
	frame.data[3] = data[3];
        if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
                perror("Write");
                return;
        }

        if (close(s) < 0) {
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

        if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
                perror("Socket");
                return;
        }

        strcpy(ifr.ifr_name, "can0" );
        ioctl(s, SIOCGIFINDEX, &ifr);

        memset(&addr, 0, sizeof(addr));
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("Bind");
                return;
        }
	if (filterId != -1)
	{
		struct can_filter rfilter[1];

		rfilter[0].can_id   = filterId;
		rfilter[0].can_mask = 0x7ff;

		setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
	}
        nbytes = read(s, &frame, sizeof(struct can_frame));

        if (nbytes < 0) {
                perror("Read");
                return;
        }
	value[0] = frame.can_id;
        for (i = 0; i < 4; i++)
                value[i+1] = frame.data[i];

        if (close(s) < 0) {
                perror("Close");
                return;
        }
}
int main(int argc, char **argv)
{
	/*
	printf("radio control service\n");
	while(1)
	{
		int value[5];
		int canSendData[4];
        	readCan(17, value);
		if (value[1] == 1)
		{
			printf("%d: %d\n", value[1], value[2]);
		}
		if (value[1] == 2)
		{
			printf("%d: %d\n", value[1], value[2]);
			canSendData[0] = round(value[2]/ 2);
			canSendData[1] = 1;
			canSendData[2] = round(value[2]/2);
			canSendData[3] = 0;
			sendCan(canSendData, 0x004);
		}
		
		if (value[1] == 4)
                {*/
			//printf("%d: %d\n", value[1], value[2]);
                        /*
			canSendData[0] = 22;
                        canSendData[1] = round(value[2] * 100 / 255);
                        canSendData[2] = value[3];
                        canSendData[3] = 2;
                        sendCan(canSendData, 0x014);
			*/
                /*}
		
	}*/
	while (1)
  {
    int data[5];
    readCan(-1,data);
    if (data[0] == 17)
    {
	    if (data[1] == 4)
	    	printf("%d: %d %d %d\n", data[1], data[2], data[3], data[4]);
    }
  }

	return 0;
}
