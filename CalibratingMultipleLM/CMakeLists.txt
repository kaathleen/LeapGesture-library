cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(calibratingLMs)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Eigen3 REQUIRED)

include_directories("${EIGEN3_INCLUDE_DIR}")

add_executable (calib calib.cpp main.cpp )
target_link_libraries (calib)
