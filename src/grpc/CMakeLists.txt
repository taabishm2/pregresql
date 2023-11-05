cmake_minimum_required(VERSION 3.8)

project(PostgresGRPC C CXX)

include(common.cmake)


set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

# Protobuf-compiler
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# gRPC
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(hw_proto "./protos/helloworld.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)
# for sundial
get_filename_component(sd_proto "./protos/sundial.proto" ABSOLUTE)
get_filename_component(sd_proto_path "${sd_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# for sundial
set(sd_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/sundial.pb.cc")
set(sd_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/sundial.pb.h")
set(sd_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/sundial.grpc.pb.cc")
set(sd_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/sundial.grpc.pb.h")
add_custom_command(
      OUTPUT "${sd_proto_srcs}" "${sd_proto_hdrs}" "${sd_grpc_srcs}" "${sd_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${sd_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${sd_proto}"
      DEPENDS "${sd_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# sd_grpc_proto
add_library(sd_grpc_proto
  ${sd_grpc_srcs}
  ${sd_grpc_hdrs}
  ${sd_proto_srcs}
  ${sd_proto_hdrs})
target_link_libraries(sd_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

#Targets greeter_[async_](client|server)

foreach(_target
  greeter_server)
  add_executable(${_target} "${_target}.cc")
  target_link_libraries(${_target}
    hw_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()

# for sundial
foreach(sd_target
  sundial_postgres_rpc_server)
  add_executable(${sd_target} "${sd_target}.cpp")
  target_link_libraries(${sd_target}
    sd_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()

add_library(client_grpc SHARED client.cc ${grpc_srcs}
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
  
target_link_libraries(client_grpc
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# for sundial
add_library(sd_client_grpc SHARED sundial_pg_rpc_client.cpp ${grpc_srcs}
  ${sd_grpc_srcs}
  ${sd_grpc_hdrs}
  ${sd_proto_srcs}
  ${sd_proto_hdrs})

target_link_libraries(sd_client_grpc
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
