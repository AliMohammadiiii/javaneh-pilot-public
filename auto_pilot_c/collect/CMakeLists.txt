

cmake_minimum_required(VERSION 3.8)
project(train C CXX)




include(../cmake/common.cmake)



include_directories("./lib/")


add_library(data_collect ./lib/data_collect.cc ./lib/data_collect.h)
target_link_libraries(data_collect
  database
  gps
  canbus
)


add_executable(collector "collector.cc")
target_include_directories(collector PRIVATE "./lib")
target_link_libraries(collector
  data_collect
  hw_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

set_target_properties( collector
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
