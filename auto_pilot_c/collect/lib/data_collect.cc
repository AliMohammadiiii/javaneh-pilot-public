
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "data_collect.h"
#include "database.h"
#include "gps.h"
#include "canbus.h"


#include "javaneh.grpc.pb.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using Tiro::Vehicle;
using Tiro::Point;

ABSL_FLAG(uint16_t, port, 50053, "Server port for the service");


class VehicleImpl final : public Vehicle::Service {
  public :
	Point* point;

  Status Info(ServerContext* context, const Point* request,
                  Point* reply) override {
    reply->set_speed(point->speed());
	reply->set_speed_direction(point->speed_direction());
    reply->set_steering_angle(point->steering_angle());
	reply->set_steering_angle_direction(point->steering_angle_direction());
    reply->set_safe_mode(point->safe_mode());
	reply->set_turn_in_place(point->turn_in_place());
    reply->set_lat(point->lat());
	reply->set_lon(point->lon());
	reply->set_auto_pilot_mode(point->auto_pilot_mode());
	reply->set_angle(point->angle());
	reply->set_mission(point->mission());
	reply->set_position_fix(point->position_fix());
    return Status::OK;
  }
};


DataCollector::DataCollector()
{
	point = new Point();
}

void DataCollector::gps(){
	Gps gps = Gps("/dev/serial0");
	gps.point = point;
	gps.read();
	// gps.read2();
}
void DataCollector::canbus(){
	Canbus canbus = Canbus("can0");
	canbus.point = point;
	canbus.readBus();
	
}

void DataCollector::runServer(uint16_t port){
	thread gps_thread(&DataCollector::gps, this);
	thread canbus_thread(&DataCollector::canbus, this);
	std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
	VehicleImpl service;
	service.point = point;

	grpc::EnableDefaultHealthCheckService(true);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();
	ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService(&service);
	// Finally assemble the server.
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;

	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
	gps_thread.join();
	canbus_thread.join();
}

void DataCollector::run()
{
	// absl::ParseCommandLine(argc, argv);
  	runServer(absl::GetFlag(FLAGS_port));
   

    // Gps g = Gps("/dev/serial0");
    // thread t = g.read();
    // while(true){
	// 	usleep(500000);
	// }
	// t.join();
	

}

