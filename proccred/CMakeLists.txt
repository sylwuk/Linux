cmake_minimum_required(VERSION 3.16)
project(proccred)

set(GET_IDSHOW_SOURCES idshow.c ../lib/error_functions.c ../lib/get_num.c ../users_groups/ugid_functions.c)
add_executable(idshow ${GET_IDSHOW_SOURCES})

target_include_directories(idshow PUBLIC ../lib)
target_include_directories(idshow PUBLIC ../users_groups)