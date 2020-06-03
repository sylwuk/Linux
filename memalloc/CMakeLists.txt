cmake_minimum_required(VERSION 3.16)
project(memalloc)

set(FREE_SBRK_SOURCES free_and_sbrk.c)

add_subdirectory(../lib lib)

add_executable(free_and_sbrk ${FREE_SBRK_SOURCES})

target_link_libraries(free_and_sbrk PUBLIC lib)
target_include_directories(free_and_sbrk PUBLIC ../lib)