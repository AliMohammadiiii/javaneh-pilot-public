
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
	int speed[2];
	int angle[2];
	int turn;
	while(1)
	{
		int value[5];
		int canSendData[4];
        	readCan(17, value);
		if (value[1] == 2)
		{
			if(1)
			{
				speed[0] = value[2];
				speed[1] = value[3];
				canSendData[0] = 21;
				canSendData[1] = value[2];
				canSendData[2] = value[3];
				canSendData[3] = 2;
				sendCan(canSendData, 0x014);
			}
		}
		if (value[1] == 3)
                {
			if(1)
			{
				angle[0] = value[2];
				angle[1] = value[3];
				canSendData[0] = 22;
				canSendData[1] = round(value[2] * 100 / 255);
				canSendData[2] = value[3];
				canSendData[3] = 2;
				sendCan(canSendData, 0x014);
			}
                }
		
		if (value[1] == 5)
		{
			if( 1 )
			{
				turn = value[3];
				canSendData[0] = value[3];
				canSendData[1] = 0;
				canSendData[2] = 0;
				canSendData[3] = 0;
				sendCan(canSendData, 0x032);
			}
		}
		
	}
	return 0;
}
