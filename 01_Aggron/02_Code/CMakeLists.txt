cmake_minimum_required(VERSION 2.8)
set (CMAKE_ECLIPSE_VERSION 4.5.1)  
UNSET(CMAKE_C_FLAGS CACHE)
SET(CMAKE_C_FLAGS "-g -O0 -Wall -W" CACHE STRING "C Flags" FORCE)

#--------------------------------------------------------------------------------------------------#

add_subdirectory(./01_HighLevel Build_HighLevel)
include_directories(Tangela_INCLUDE_DIRS)

#--------------------------------------------------------------------------------------------------# 

