cmake_minimum_required(VERSION 3.10.0)
project(CerveurusSample C)


# build cerveurus dependency
add_subdirectory(ext/cerveurus)

set(CMAKE_C_FLAGS "-O0 -g -Wall -fsanitize=address")
add_subdirectory(src)
