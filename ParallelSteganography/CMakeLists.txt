cmake_minimum_required(VERSION 3.15)
project(ParallelSteganography)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED) # after project()

include_directories(/usr/local/include)

set(CMAKE_CXX_FLAGS_DEBUG "-lomp -fopenmp")
set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")

add_executable(ParallelSteganography main.cpp lsb.cc lsb.h rc4.cc rc4.h steganography.cc steganography.h)
#At the end
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(ParallelSteganography ${OpenCV_LIBS})

