cmake_minimum_required(VERSION 3.16)
project(time)

set(GET_CALENDAR_TIME_SOURCES calendar_time.c ../lib/error_functions.c ../lib/get_num.c)
add_executable(calendar_time ${GET_CALENDAR_TIME_SOURCES})

target_include_directories(calendar_time PUBLIC ../lib)