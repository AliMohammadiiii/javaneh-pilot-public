
cmake_minimum_required(VERSION 3.8)
project(JavanehAutoPilot C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

include(cmake/common.cmake)
include(cmake/sqlite.cmake)

# Proto file
# get_filename_component(hw_proto "./protos/javaneh.proto" ABSOLUTE)
# get_filename_component(hw_kuksa_proto "./protos/kuksa.proto" ABSOLUTE)
# get_filename_component(hw_proto_path "${hw_proto}" PATH)




# Generated sources
# set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/javaneh.pb.cc")
# set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/javaneh.pb.h")
# set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/javaneh.grpc.pb.cc")
# set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/javaneh.grpc.pb.h")


# set(hw_kuksa_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/kuksa.pb.cc")
# set(hw_kuksa_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/kuksa.pb.h")
# set(hw_kuksa_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/kuksa.grpc.pb.cc")
# set(hw_kuksa_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/kuksa.grpc.pb.h")


# set(hw_proto_pysrcs "${CMAKE_CURRENT_BINARY_DIR}/javaneh_pb2_grpc.py")
# set ( PROTO_PY ${CMAKE_CURRENT_LIST_DIR}/javaneh_pb2.py ${CMAKE_CURRENT_LIST_DIR}/javaneh_pb2_grpc.py )

# add_custom_command(
#       OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
#       COMMAND ${_PROTOBUF_PROTOC}
#       ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
#         --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
#         -I "${hw_proto_path}"
#         --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
#         "${hw_proto}"
#       DEPENDS "${hw_proto}")



# add_custom_command(
#       OUTPUT "${hw_kuksa_proto_srcs}" "${hw_kuksa_proto_hdrs}" "${hw_kuksa_grpc_srcs}" "${hw_kuksa_grpc_hdrs}"
#       COMMAND ${_PROTOBUF_PROTOC}
#       ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
#         --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
#         -I "../../../grpc/third_party/protobuf/src/"
#         -I "${hw_proto_path}"
#         --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
#         "${hw_kuksa_proto}"
#       DEPENDS "${hw_kuksa_proto}")      

# find_package(Python...)
# find_package(PythonLibs 3.8 REQUIRED)
# find_package(Boost COMPONENTS python REQUIRED)      
# find_package(Boost COMPONENTS python38 REQUIRED)



# add_custom_command(
#   OUTPUT ${PROTO_PY}
#   COMMAND ${PYTHON_EXECUTABLE}
#   # COMMAND python38
  
#   ARGS -m grpc_tools.protoc
#   --python_out=./
#   --grpc_python_out=./
#   --pyi_out=./
#     -I "${hw_proto_path}"
#     "${hw_proto}"
#   DEPENDS "${hw_proto}")

# add_custom_target ( proto_py ALL DEPENDS ${PROTO_PY} )

# install( PROGRAMS ${PROTO_PY} COMPONENT mycomponent DESTINATION ./bin/ )

# python3 -m grpc_tools.protoc --python_out=helloworld --grpc_python_out=helloworld --pyi_out=helloworld -I ../protos ../protos/helloworld.proto

# Include generated *.pb.h files


find_library(libconfig++ libconfig++)
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
find_library(HIREDIS_LIBRARIES NAMES hiredis)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("./lib/")
# include_directories("./lib/libnabo/nabo/")

# hw_grpc_proto
# add_library(hw_grpc_proto
#   ${hw_grpc_srcs}
#   ${hw_grpc_hdrs}
#   ${hw_proto_srcs}
#   ${hw_proto_hdrs})
# target_link_libraries(hw_grpc_proto
#   ${_REFLECTION}
#   ${_GRPC_GRPCPP}
#   ${_PROTOBUF_LIBPROTOBUF})


#   add_library(hw_kuksa_grpc_proto
#   ${hw_kuksa_grpc_srcs}
#   ${hw_kuksa_grpc_hdrs}
#   ${hw_kuksa_proto_srcs}
#   ${hw_kuksa_proto_hdrs})
# target_link_libraries(hw_kuksa_grpc_proto
#   ${_REFLECTION}
#   ${_GRPC_GRPCPP}
#   ${_PROTOBUF_LIBPROTOBUF})




# add_subdirectory(./lib/libnabo/)

add_library(conf ./lib/conf.cc ./lib/conf.h)
target_link_libraries(conf
  config++
)

add_library(kclient ./lib/kclient.cc ./lib/kclient.h)
# target_link_libraries(kclient
#   # hw_grpc_proto
#   # hw_kuksa_grpc_proto
# )
target_include_directories(kclient PRIVATE ${HIREDIS_INCLUDE_DIR})
target_link_libraries(kclient PRIVATE ${HIREDIS_LIBRARIES})

add_library(helper ./lib/helper.cc ./lib/helper.h)
target_link_libraries(helper
  # hw_grpc_proto
  # hw_kuksa_grpc_proto
  kclient

)

# add_library(database ./lib/database.cc ./lib/database.h)
# target_link_libraries(database
# 	sqlite3
# 	# hw_grpc_proto
# 	)

# add_library(points ./lib/points.cc ./lib/points.h ./lib/libnabo/nabo/nabo.h)
# target_link_libraries(points
# 	nabo
# 	database
# 	hw_grpc_proto
# )

# add_library(vehicle_client ./lib/vehicle_client.cc ./lib/vehicle_client.h)
# target_link_libraries(vehicle_client
#   hw_grpc_proto

# )


# add_library(gps ./lib/gps.cc ./lib/gps.h)
# target_link_libraries(gps
# 	serial
# 	nmea
#   hw_grpc_proto
# )


add_library(canbus ./lib/canbus.cc ./lib/canbus.h)
target_link_libraries(canbus
        # hw_grpc_proto
)




# add_subdirectory(./train)
# add_subdirectory(./drive)
# add_subdirectory(./collect)
# add_subdirectory(./kuksa)
add_subdirectory(./kuksa2can)
# add_subdirectory(./control)
# add_subdirectory(./lidar)
# add_subdirectory(./gps)
add_subdirectory(./brakeSystem)
add_subdirectory(./bucketManagement)


add_subdirectory(./can2kuksa)
# add_subdirectory(./utilityControl)


