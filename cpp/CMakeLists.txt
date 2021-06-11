# cmake 最低版本需求
cmake_minimum_required(VERSION 3.13)
# 工程名称
project(grpc_client)
# 添加目标可执行文件
add_executable(grpc_client ./src/main.cpp ./src/grpc/worker.cpp)