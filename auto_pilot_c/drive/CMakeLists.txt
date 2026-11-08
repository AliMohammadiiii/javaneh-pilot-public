

cmake_minimum_required(VERSION 3.8)
project(drive C CXX)




include(../cmake/common.cmake)

# include_directories("./lib/libnabo/nabo/")
# add_subdirectory(./lib/libnabo/)

include_directories("./lib/")


# add_library(points ./lib/points.cc ./lib/points.h ./lib/libnabo/nabo/nabo.h)
# target_link_libraries(points
# 	nabo
# 	database
# 	hw_grpc_proto
# )

add_library(drive ./lib/drive.cc ./lib/drive.h)
target_link_libraries(drive
  database
  canbus
  points
  hw_grpc_proto
  kclient
  hw_kuksa_grpc_proto
)


add_executable(driver "driver.cc")
target_include_directories(driver PRIVATE "./lib")
target_link_libraries(driver
  drive
  helper
  hw_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

set_target_properties( driver
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
