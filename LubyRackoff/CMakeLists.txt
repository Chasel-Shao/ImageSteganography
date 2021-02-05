cmake_minimum_required(VERSION 3.15)
project(lubyrack)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include/botan-2)
link_directories(/usr/local/lib)

add_executable(lubyrack main.cpp lubyrackoff.cc lubyrackoff.h)


target_link_libraries(lubyrack botan-2)