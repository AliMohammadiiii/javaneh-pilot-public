

cmake_minimum_required(VERSION 3.8)
project(lidar C CXX)

add_custom_target(RPLidrLib ALL
  COMMAND make
  # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../lidar/lib/sdk/
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/
)


# include_directories(${CMAKE_CURRENT_BINARY_DIR}/../../lidar/lib/sdk/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/include/)

# link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../lidar/lib/sdk/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/)
add_library(lidar ./lib/lidar.cc ./lib/lidar.h)

add_dependencies(lidar RPLidrLib)

target_link_libraries(lidar
  sl_lidar_sdk
  canbus
  points
  kclient
  hw_grpc_proto
  hw_kuksa_grpc_proto
)



add_executable(scan_lidar "scan_lidar.cc")
target_include_directories(scan_lidar PRIVATE "./lib")
target_link_libraries(scan_lidar
  lidar
  hw_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

set_target_properties(scan_lidar
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
