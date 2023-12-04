cmake_minimum_required(VERSION 2.8)
set (CMAKE_ECLIPSE_VERSION 4.5.1)    
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1) 

# Options
option(COVERAGE "Enable code coverage" OFF)

if (COVERAGE)
    add_definitions(--coverage)    
endif()

#--------------------------------------------------------------------------------------------------#
 
project(Library_LibLidarLms151) 

#--------------------------------------------------------------------------------------------------#
set (SRCS  
	./src/Library_LibLidarLms151.c
)

set (TST_SRCS  
	./test/TV_Library_LibLidarLms151.c
)   

include_directories ( 
	../../02_Middleware/Library_StdLib/inc   
	../../03_Driver/Driver_TcpSocketDriver/inc
	./inc 
) 
 
if(NOT TARGET Lib_TcpSocketDriver)
      message(STATUS "Lib_TcpSocketDriver : To be constructed ")
	  add_subdirectory(../../03_Driver/Driver_TcpSocketDriver build_Tcp)  
else(TARGET Lib_TcpSocketDriver)
      message(STATUS "Lib_TcpSocketDriver : target exists ") 
endif()  

#--------------------------------------------------------------------------------------------------#

add_executable(TV_LibLidarLms151 ${TST_HEADERS} ${SRCS} ${TST_SRCS})   
target_link_libraries(TV_LibLidarLms151 Lib_TcpSocketDriver m  --coverage)  
set_target_properties(TV_LibLidarLms151 PROPERTIES EXCLUDE_FROM_ALL TRUE) 
 
#--------------------------------------------------------------------------------------------------#

add_library(Lib_LibLidarLms151 STATIC ${SRCS} )    
target_link_libraries(Lib_LibLidarLms151 Lib_TcpSocketDriver)  

#--------------------------------------------------------------------------------------------------#

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(docLibLidarLms151
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

#--------------------------------------------------------------------------------------------------#
