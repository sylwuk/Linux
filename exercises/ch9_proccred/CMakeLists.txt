cmake_minimum_required(VERSION 3.16)
project(ch9_proccred)

set(GET_INITGROUPS_SOURCES initgroups.c ../../lib/error_functions.c ../../lib/get_num.c ../../users_groups/ugid_functions.c)
add_executable(initgroups ${GET_INITGROUPS_SOURCES})

target_include_directories(initgroups PUBLIC ../../lib)
target_include_directories(initgroups PUBLIC ../../users_groups)