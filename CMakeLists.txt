cmake_minimum_required(VERSION 3.27)
project(ftrace C)

set(CMAKE_C_STANDARD 11)

add_executable(ftrace
        hello.c
        hook_only.c
        hashtable.h
        hashtable.c)
