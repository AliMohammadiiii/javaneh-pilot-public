#include <grpc++/grpc++.h>
#include "javaneh.grpc.pb.h"
#include "vehicle_client.h"
#include <iostream>

using namespace std;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using Tiro::Point;
using Tiro::Vehicle;


VehicleClient::VehicleClient(std::shared_ptr<Channel> channel)
       : stub_(Vehicle::NewStub(channel)) {
       }
Point VehicleClient::GetInfo(){
    Point request;
    Point reply;
    ClientContext context;
    Status status = stub_->Info(&context, request, &reply);
    if (status.ok()) {
	    return reply;
    } else {
      return reply;
    }
}

// class VehicleClient {
//  public:
//   VehicleClient(std::shared_ptr<Channel> channel)
//       : stub_(Vehicle::NewStub(channel)) {}
//   Point GetInfo() {
//     Point request;
//     Point reply;
//     ClientContext context;
//     Status status = stub_->Info(&context, request, &reply);
//     if (status.ok()) {
// 	  return reply;
//     } else {
//       std::cout << status.error_code() << ": " << status.error_message()
//                 << std::endl;
//       return reply;
//     }
//   }
//  private:
//   std::unique_ptr<Vehicle::Stub> stub_;
// };
