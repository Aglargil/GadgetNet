cmake_minimum_required(VERSION 3.10)
project(GADGETNET)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)

ADD_DEFINITIONS(-DDEBUGMODE)

# 开启测试
enable_testing()

# 添加测试目录
add_subdirectory(test)

add_subdirectory(example)