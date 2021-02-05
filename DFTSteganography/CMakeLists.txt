cmake_minimum_required(VERSION 3.15)
project(DFTSteg)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(DFTSteg main.cpp WatermarkText.cpp WatermarkText.h lsb.cc lsb.h)

#At the end
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(DFTSteg ${OpenCV_LIBS})