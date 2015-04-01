cmake_minimum_required(VERSION 3.1)
PROJECT ( FifoBuffer C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES FifoBuffer_test.c
FifoBuffer.h)

add_executable(FifoBuffer ${SOURCE_FILES})