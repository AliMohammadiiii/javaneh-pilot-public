

cmake_minimum_required(VERSION 3.8)
project(train C CXX)




include(../cmake/common.cmake)

include_directories("./")

add_library(train ./train.cc ./train.h)
target_link_libraries(train
  database
  points
  kclient
  hw_kuksa_grpc_proto
  
)


add_executable(trainer "trainer.cc")
target_include_directories(trainer PRIVATE "./lib")
target_link_libraries(trainer
  train
  hw_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

set_target_properties( trainer
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
