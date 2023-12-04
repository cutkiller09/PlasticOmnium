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
 
project(Driver_TcpSocketDriver) 

#--------------------------------------------------------------------------------------------------#
set (SRCS  
	./src/Driver_TcpSocketDriver.c
)

set (TST_SRCS 
	./test/TI_Driver_TcpSocketDriver.c
) 

include_directories (
	../../02_Middleware/Library_StdLib/inc  
	./inc 
)   

#--------------------------------------------------------------------------------------------------#
if(NOT TARGET testTcpSocketDriver) 
	add_executable(testTcpSocketDriver ${SRCS} ${TST_SRCS}) 
	target_link_libraries(testTcpSocketDriver    
					--coverage
					)   
	set_target_properties(testTcpSocketDriver PROPERTIES EXCLUDE_FROM_ALL TRUE) 
endif()      


#--------------------------------------------------------------------------------------------------#

if(NOT TARGET Lib_TcpSocketDriver)
	add_library(Lib_TcpSocketDriver STATIC ${SRCS} )  
endif()    

#--------------------------------------------------------------------------------------------------#

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(docTcpSocketDriver
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

#--------------------------------------------------------------------------------------------------#
