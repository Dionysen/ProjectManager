cmake_minimum_required(VERSION 3.0.0)
project(pm VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
include(CTest)
enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(includes)
add_executable(pm projectmanager.h projectmanager.cpp main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
