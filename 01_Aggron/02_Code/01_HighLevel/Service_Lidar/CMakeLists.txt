cmake_minimum_required(VERSION 2.8)
set (CMAKE_ECLIPSE_VERSION 4.5.1)   
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")

# Options
option(COVERAGE "Enable code coverage" OFF)

if (COVERAGE)
    add_definitions(--coverage)    
endif()

# Options
option(DEBUG "Enable debug" OFF)

if (COVERAGE)
    add_definitions(-g)    
endif()

set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1) 

#--------------------------------------------------------------------------------------------------#
 
project(Service_Lidar) 

#--------------------------------------------------------------------------------------------------#
set (SRCS 
	../../01_HighLevel/Service_Log/src/Service_Log.c
	../../02_Middleware/Library_StdLib/src/Library_StdLib.c 
	./src/Service_Lidar.c	
)

set (TI_SRCS 
	./test/stub/Library_LibLidarLms151.c
	./test/TI_Service_Lidar.c
)   

set (TV_SRCS 
	./test/TV_Service_Lidar.c
)      
 
if(NOT TARGET Library_LibLidarLms151)
      message(STATUS "Service_Lidar : Lib_LibLidarLms151 target creation")
	  add_subdirectory(../../02_Middleware/Library_LibLidarLms151 build_libLidarLms151)
 else(TARGET Library_LibLidarLms151)
        message(STATUS "Service_Lidar : Lib_LibLidarLms151 target exists.")
endif()    

#--------------------------------------------------------------------------------------------------#

add_executable(TI_Lidar ${TI_SRCS} ${SRCS} )   
target_link_libraries(TI_Lidar --coverage)  
set_target_properties(TI_Lidar PROPERTIES EXCLUDE_FROM_ALL TRUE) 


target_include_directories (TI_Lidar PRIVATE
	../../01_HighLevel/Service_Log/inc
	../../02_Middleware/Library_StdLib/inc  
	../../03_Driver/Driver_TcpSocketDriver/inc 
	./test/stub
	./inc
)   

add_executable(TV_Lidar ${SRCS} ${TV_SRCS} )    
target_link_libraries(TV_Lidar 
					Lib_LibLidarLms151 
					--coverage)  
set_target_properties(TV_Lidar PROPERTIES EXCLUDE_FROM_ALL TRUE)  


target_include_directories (TV_Lidar PRIVATE
	../../01_HighLevel/Service_Log/inc
	../../02_Middleware/Library_StdLib/inc  
	../../02_Middleware/Library_LibLidarLms151/inc 
	../../03_Driver/Driver_TcpSocketDriver/inc 
	./inc
)   
#--------------------------------------------------------------------------------------------------#

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(docLidar
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

#--------------------------------------------------------------------------------------------------#
