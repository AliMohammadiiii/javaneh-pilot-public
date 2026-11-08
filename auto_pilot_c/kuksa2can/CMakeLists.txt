

cmake_minimum_required(VERSION 3.8)
project(kuk2can C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include(../cmake/common.cmake)
include_directories("./lib/")


add_library(kuk2can ./lib/kuk2can.cc ./lib/kuk2can.h)
target_link_libraries(kuk2can
  # hw_grpc_proto
  helper
  canbus
  kclient
  helper
  ${JSONCPP_LIBRARIES}
)


add_executable(kuksa2can "kuksa2can.cc")
target_include_directories(kuksa2can PRIVATE "./lib")
target_link_libraries(kuksa2can
  kclient

  canbus
  kuk2can
  helper
  # hw_kuksa_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  ${JSONCPP_LIBRARIES}
)

set_target_properties( kuksa2can
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
