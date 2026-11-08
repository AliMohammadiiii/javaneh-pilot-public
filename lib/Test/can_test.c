#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <math.h>
int main(void) {
  int s;
  int sW;
  struct sockaddr_can addr;
  struct can_frame frame;

  // Create a socket for CAN communication.
  s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (s < 0) {
    perror("socket");
    exit(1);
  }
sW = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  // Bind the socket to the CAN bus.
  addr.can_family = PF_CAN;
  addr.can_ifindex = 0;
  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }
  if (bind(sW, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }
/*  
  // Set the filter.
    struct can_filter rfilter[1];
  rfilter[0].can_id = 0x011;
  rfilter[0].can_mask = 0x7FF;
  setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

while(1)
{
  // Read a CAN frame from the bus.
  memset(&frame, 0, sizeof(frame));
  int len = read(s, &frame, sizeof(frame));
  if (len < 0) {
    perror("read");
    exit(1);
  }

  // Print the CAN frame data.
  // printf("%d ", frame.can_id);
  if (frame.data[0] == 2 || frame.data[0] == 4)
  { 
  for (int i = 0; i < 3; i++) {
    printf("%d ", frame.data[i]);
  }
  }
  printf("\n");

int ch = frame.data[0];
int speed = frame.data[1];
int direction = frame.data[2];
memset(&frame, 0, sizeof(frame));
if (ch == 2)
{
	frame.can_id = 0x014;
	frame.can_dlc = 8;
	frame.data[0] = 21;
	frame.data[1] = speed;
	frame.data[2] = direction;
	frame.data[3] = 2;
	printf("%d: %d %d %d %d\n",frame.can_id, frame.data[0], frame.data[1], frame.data[2], frame.data[3]);
	if (write(s, &frame, sizeof(frame)) < 0) {
		printf("hi");
		perror("write");
		exit(1);
	}
	else
	{
		printf("%d: %d %d %d %d\n",frame.can_id, frame.data[0], frame.data[1], frame.data[2], frame.data[3]);
	}

}
if (ch == 4)
{
        frame.can_id = 0x014;
        frame.can_dlc = 8;
        frame.data[0] = 22;
        frame.data[1] = round(speed/250*100);
        frame.data[2] = direction;
        frame.data[3] = 2;
        if (write(s, &frame, sizeof(frame)) < 0) {
                perror("write");
                exit(1);
        }
	        else
        {
                printf("%d: %d %d %d %d",frame.can_id, frame.data[0], frame.data[1], frame.data[2], frame.data[3]);
        }
}
}
*/
  // Write a CAN frame to the bus.
  frame.can_id = 0x014;
  frame.can_dlc = 8;
  for (int i = 0; i < frame.can_dlc; i++) {
    frame.data[i] = i;
  }
  if (write(s, &frame, sizeof(frame)) < 0) {
    perror("write");
    exit(1);
  }

  close(s);
  return 0;
}

