cmake_minimum_required(VERSION 3.15)
project(WaveletSteganography)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(WaveletSteganography main.cpp wavelettransform.cc wavelettransform.h lsb.h lsb.cc)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(WaveletSteganography ${OpenCV_LIBS})