cmake_minimum_required(VERSION 2.8)
#project name
PROJECT(sslang)
#head file path
INCLUDE_DIRECTORIES(
include
)
#source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
#set environment variable
SET(SSLANG
${DIR_SRCS}
)
#set extern libraries
#add executable file
ADD_EXECUTABLE(sslang ${SSLANG})
#add link library

set(CMAKE_CXX_FLAGS "-std=c++11 -g -pipe -Wall -Wextra")
