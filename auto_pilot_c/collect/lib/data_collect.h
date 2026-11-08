#ifndef Train_H
#define Train_H
#endif

#include "javaneh.grpc.pb.h"

using Tiro::Point;


class DataCollector
{
	private:
		Point *point;
	public:
		DataCollector();
		void run();
		void gps();
		void canbus();
		void runServer(uint16_t port);
};
