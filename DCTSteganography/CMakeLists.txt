cmake_minimum_required(VERSION 3.15)
project(JPGSteg)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)

include_directories(/usr/local/include/)
#set(CMAKE_CXX_FLAGS_DEBUG "-lomp -fopenmp")
#SET(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")

add_executable(DCTStego main.cpp
               JpegUtils.cpp JpegUtils.h
               StegUtils.cpp StegUtils.h
               Utils.cpp Utils.h)

target_link_libraries(DCTStego libjpeg.a)

#At the end
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(DCTStego ${OpenCV_LIBS})