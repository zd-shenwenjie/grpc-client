# 指定cmake最小版本
cmake_minimum_required(VERSION 3.5.1)
# 设置项目名称
project(RouteGuide C CXX)
# grpc common cmake
include(./common.cmake)

# 设置路径
get_filename_component(wrappers_proto "${CMAKE_CURRENT_LIST_DIR}/proto/wrappers.proto" ABSOLUTE)
get_filename_component(manager_proto "${CMAKE_CURRENT_LIST_DIR}/proto/manager.proto" ABSOLUTE)
get_filename_component(networkif_proto "${CMAKE_CURRENT_LIST_DIR}/proto/networkif.proto" ABSOLUTE)
get_filename_component(wrappers_proto_path "${wrappers_proto}" PATH)
get_filename_component(manager_proto_path "${manager_proto}" PATH)
get_filename_component(networkif_proto_path "${networkif_proto}" PATH)

# 设置变量
set(wrappers_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/wrappers.pb.cc")
set(wrappers_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/wrappers.pb.h")
set(wrappers_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/wrappers.grpc.pb.cc")
set(wrappers_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/wrappers.grpc.pb.h")
set(manager_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/manager.pb.cc")
set(manager_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/manager.pb.h")
set(manager_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/manager.grpc.pb.cc")
set(manager_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/manager.grpc.pb.h")
set(networkif_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/networkif.pb.cc")
set(networkif_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/networkif.pb.h")
set(networkif_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/networkif.grpc.pb.cc")
set(networkif_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/networkif.grpc.pb.h")

# 添加自定义命令
add_custom_command(
  # 目标文件名称 OUTPUT output1 [output2 ...]
  OUTPUT 
    "${wrappers_proto_srcs}" 
    "${wrappers_proto_hdrs}" 
    "${wrappers_grpc_srcs}" 
    "${wrappers_grpc_hdrs}"
    "${manager_proto_srcs}" 
    "${manager_proto_hdrs}" 
    "${manager_grpc_srcs}" 
    "${manager_grpc_hdrs}"
    "${networkif_proto_srcs}" 
    "${networkif_proto_hdrs}" 
    "${networkif_grpc_srcs}" 
    "${networkif_grpc_hdrs}"
  # 需要执行的命令 COMMAND command1 [ARGS] [args1...]
  COMMAND ${_PROTOBUF_PROTOC}
  ARGS 
    # 生成 C++ 头文件和源代码
    --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    # 查找 *.proto 文件的目录
    --proto_path "${wrappers_proto_path}"
    --proto_path "${manager_proto_path}"
    --proto_path "${networkif_proto_path}"
    # 设置插件
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    # 一个或多个 *.proto 文件
    "${wrappers_proto}" "${manager_proto}" "${networkif_proto}"
  # 执行命令时需要的依赖 [DEPENDS [depends...]]
  DEPENDS "${wrappers_proto}" "${manager_proto}" "${networkif_proto}"
)

# 设置包含的目录 *.pb.h
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# 生成proto静态库
add_library(
  grpc_proto
  ${wrappers_proto_srcs}
  ${wrappers_proto_hdrs}
  ${wrappers_grpc_srcs}
  ${wrappers_grpc_hdrs}
  ${manager_proto_srcs}
  ${manager_proto_hdrs}
  ${manager_grpc_srcs}
  ${manager_grpc_hdrs}
  ${networkif_proto_srcs}
  ${networkif_proto_hdrs}
  ${networkif_grpc_srcs}
  ${networkif_grpc_hdrs}
)
# 设置proto需要链接的库
target_link_libraries(
  grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# 生成adapter静态库
add_library(
  grpc_adapter
  "${CMAKE_CURRENT_LIST_DIR}/src/grpc/adaptee.h"
  "${CMAKE_CURRENT_LIST_DIR}/src/grpc/adaptee.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/grpc/adapter.h"
  "${CMAKE_CURRENT_LIST_DIR}/src/grpc/adapter.cpp"
)
# 设置target需要链接的库
target_link_libraries(
  grpc_adapter 
  grpc_service
)

 # 生成service静态库
add_library(
  grpc_service
  "${CMAKE_CURRENT_LIST_DIR}/src/service/observer.h"
  "${CMAKE_CURRENT_LIST_DIR}/src/service/observer.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/service/provider.h"
  "${CMAKE_CURRENT_LIST_DIR}/src/service/provider.cpp"
)
# 设置service需要链接的库
target_link_libraries(
  grpc_service 
  grpc_adapter
)

# 生成可执行文件
add_executable(grpc-client "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")
# 设置target需要链接的库
target_link_libraries(
  grpc-client 
  grpc_adapter 
  grpc_service
  grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)