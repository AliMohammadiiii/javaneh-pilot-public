

cmake_minimum_required(VERSION 3.8)
project(kuk2can C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include_directories("../lib")
include(../cmake/common.cmake)
include_directories("./lib/")
include_directories("./etc/")
add_library(brakeObj ./etc/brakeObj.cc ./etc/brakeObj.h)
target_link_libraries(brakeObj
  # hw_grpc_proto
)

add_library(brakeRow ./etc/brakeRow.cc ./etc/brakeRow.h)
target_link_libraries(brakeRow
  # hw_grpc_proto
  brakeObj
)

add_library(brakeSystem ./lib/brakeSystem.cc ./lib/brakeSystem.h)
target_link_libraries(brakeSystem
  # hw_grpc_proto
  brakeObj
  brakeRow
  kclient
  ${JSONCPP_LIBRARIES}
)


add_executable(braker "braker.cc")
target_include_directories(braker PRIVATE "./lib")
target_link_libraries(braker
  kclient
  brakeObj
  brakeRow
  brakeSystem
  # hw_kuksa_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  ${JSONCPP_LIBRARIES}
)

set_target_properties( braker
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
