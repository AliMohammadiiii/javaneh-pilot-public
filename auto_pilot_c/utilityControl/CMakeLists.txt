

cmake_minimum_required(VERSION 3.8)
project(kuk2can C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include_directories("../lib")
include(../cmake/common.cmake)
include_directories("./lib/")
include_directories("./etc/")


add_library(utilityControl ./lib/utilityControl.cc ./lib/utilityControl.h)
target_link_libraries(utilityControl
  hw_grpc_proto

  kclient
  ${JSONCPP_LIBRARIES}
)


add_executable(utilityControler "utilityControler.cc")
target_include_directories(utilityControler PRIVATE "./lib")
target_link_libraries(utilityControler
  kclient
  utilityControl
  hw_kuksa_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  ${JSONCPP_LIBRARIES}
)

set_target_properties( utilityControler
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
