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
 
project(Protocol_CanOpen) 

#--------------------------------------------------------------------------------------------------#
set (SRCS 
	./src/Library_PrivateCanOpenPCanWrapper.c
	./src/Library_PrivateCanOpenXtppSerialWrapper.c
	./src/Library_PrivateCanOpenComWrapper.c
	./src/Library_PrivateCanOpenSdoManage.c
	./src/Library_PrivateCanOpenNmtManage.c
	./src/Library_PrivateCanOpenPdoManage.c
	./src/Library_PrivateCanOpenSyncManage.c
	./src/Protocol_CanOpen.c
)

set (TST_SRCS 
	./test/TI_Protocol_CanOpen.c
)  

include_directories (
	../../02_Middleware/Library_StdLib/inc
	../../03_Driver/Driver_SerialDriver/inc
	../../03_Driver/Driver_PCanDriver/inc
	./inc 
) 

add_subdirectory(../../03_Driver Build_Driver)

if(NOT TARGET Lib_SerialDriver)
      message(STATUS "Protocol_CanOpen : Lib_SerialDriver target creation")
	  add_subdirectory(../../03_Driver/Driver_SerialDriver build_Serial)
 else(TARGET Lib_SerialDriver)
        message(STATUS "Protocol_CanOpen : Lib_SerialDriver target exists.")
endif()  

if(NOT TARGET LibPCanDriver)
      message(STATUS "Protocol_CanOpen : LibPCanDriver target creation")
	  add_subdirectory(../../03_Driver/Driver_PCanDriver build_PCan)
 else(TARGET LibPCanDriver)
        message(STATUS "Protocol_CanOpen : LibPCanDriver target exists.")
endif()  

#--------------------------------------------------------------------------------------------------#

add_executable(testCanOpen ${SRCS} ${TST_SRCS} )  
target_link_libraries(testCanOpen Lib_SerialDriver Lib_PCanDriver --coverage )  
set_target_properties(testCanOpen PROPERTIES EXCLUDE_FROM_ALL TRUE) 
 
#--------------------------------------------------------------------------------------------------#

add_library(Lib_CanOpen STATIC ${SRCS} )    
target_link_libraries(Lib_CanOpen Lib_SerialDriver Lib_PCanDriver m)  

#--------------------------------------------------------------------------------------------------#

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(docCanOpen
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

#--------------------------------------------------------------------------------------------------#
