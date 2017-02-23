cmake_minimum_required(VERSION 3.6)
project(Strings)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

link_directories(/home/morell/Рабочий стол/2.4/akos/Str/include)

set(SOURCE_FILES strings.h realpath.c )
include_directories(/home/morell/Рабочий стол/2.4/akos/Str/include)
add_executable(Strings ${SOURCE_FILES}  )