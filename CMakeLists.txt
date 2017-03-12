cmake_minimum_required(VERSION 3.6)
project(Strings)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES strings.h grep.h test.h realpath.h realpath.c)
add_executable(Strings ${SOURCE_FILES}  )
