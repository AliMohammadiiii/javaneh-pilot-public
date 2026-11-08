#ifndef Train_H
#define Train_H
#endif

#include <grpc++/grpc++.h>
#include "javaneh.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using Tiro::Point;
using Tiro::Vehicle;

class VehicleClient
{
	private:
  		std::unique_ptr<Vehicle::Stub> stub_;
	public:
		VehicleClient();
		VehicleClient(std::shared_ptr<Channel> channel);
		Point GetInfo();
};
