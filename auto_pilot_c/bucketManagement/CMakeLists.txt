

cmake_minimum_required(VERSION 3.8)
project(kuk2can C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include_directories("../lib")
include_directories("../kuksa")
include(../cmake/common.cmake)
include_directories("./lib/")


add_library(bucketManagement ./lib/bucketManagement.cc ./lib/bucketManagement.h)
target_link_libraries(bucketManagement
  # hw_grpc_proto
  kclient
  ${JSONCPP_LIBRARIES}
)


add_executable(bucketManagemer "bucketManager.cc")
target_include_directories(bucketManagemer PRIVATE "./lib")
target_link_libraries(bucketManagemer
  kclient
  bucketManagement
  # hw_kuksa_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  ${JSONCPP_LIBRARIES}
)

set_target_properties( bucketManagemer
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
