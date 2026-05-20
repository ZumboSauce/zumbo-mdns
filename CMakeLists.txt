set(CMAKE_C_OMPILER "gcc")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project( zumbomdns DESCRIPTION "Zumbosauce's mdns library" LANGUAGES C )

cmake_minimum_required(VERSION 3.15)

add_library( zmdns STATIC src/zmdns.c )
target_include_directories( zmdns PUBLIC ./include )
