cmake_minimum_required(VERSION 3.15)
project(IntegerWavelet)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(IntegerWavelet main.cpp inthaar2.cc inthaar2.h)


include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(IntegerWavelet ${OpenCV_LIBS})