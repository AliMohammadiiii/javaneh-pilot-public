

cmake_minimum_required(VERSION 3.8)
project(gpsd C CXX)




include(../cmake/common.cmake)
include_directories("./lib/")

include_directories("./lib/libnmea/include/")
add_subdirectory(./lib/libnmea/)




add_library(gps ./lib/gps.cc ./lib/gps.h)
target_link_libraries(gps
	serial
  kclient
  hw_kuksa_grpc_proto
	nmea
  # hw_grpc_proto
)


add_executable(gpsd "gpsd.cc")
target_include_directories(gpsd PRIVATE "./lib")
target_link_libraries(gpsd
config++
  conf
  gps
  hw_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

set_target_properties( gpsd
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
