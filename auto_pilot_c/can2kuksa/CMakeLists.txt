

cmake_minimum_required(VERSION 3.8)
project(kuk2can C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include_directories("../lib")
include_directories("../kuksa")
include(../cmake/common.cmake)
include_directories("./lib/")
include_directories("./etc/")
add_library(KuksaItem ./etc/KuksaItem.cc ./etc/KuksaItem.h)
target_link_libraries(KuksaItem
  # hw_grpc_proto
  kclient
)

add_library(canMessage ./etc/canMessage.cc ./etc/canMessage.h)
target_link_libraries(canMessage
  # hw_grpc_proto
  KuksaItem
  kclient
)

add_library(can2kuksa ./lib/can2kuksa.cc ./lib/can2kuksa.h)
target_link_libraries(can2kuksa
  helper
  # hw_grpc_proto
  KuksaItem
  canMessage
  kclient
  ${JSONCPP_LIBRARIES}
)


add_executable(can2kuk "can2kuk.cc")
target_include_directories(can2kuk PRIVATE "./lib")
target_link_libraries(can2kuk
  helper
  kclient
  KuksaItem
  canMessage
  can2kuksa
  # hw_kuksa_grpc_proto
  canbus
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  ${JSONCPP_LIBRARIES}
)

set_target_properties( can2kuk
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)