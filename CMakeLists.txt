cmake_minimum_required(VERSION 3.6)
project(Strings)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES strings.h grep.c grep.h test.h)
add_executable(Strings ${SOURCE_FILES}  )
