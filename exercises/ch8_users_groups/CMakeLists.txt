cmake_minimum_required(VERSION 3.16)
project(ch8_users_groups)

set(GET_PWNAM_SOURCES getpwnam.c ../../lib/error_functions.c ../../lib/get_num.c)
add_executable(getpwnam ${GET_PWNAM_SOURCES})

target_include_directories(getpwnam PUBLIC ../../lib)