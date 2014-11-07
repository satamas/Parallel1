cmake_minimum_required(VERSION 2.8.4)
project(hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")

set(SOURCE_FILES main.cpp Executor.cpp ExecutorService.cpp )
set(HEADER_FILES Executor.h ExecutorService.h Callable.h Future.h FutureTask.h)

add_executable(hw1 ${SOURCE_FILES})