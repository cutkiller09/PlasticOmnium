cmake_minimum_required(VERSION 2.8)
set (CMAKE_ECLIPSE_VERSION 4.5.1)   
 
# Options
option(COVERAGE "Enable code coverage" OFF)

if (COVERAGE)
    message(STATUS "COVERAGE option activated")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")    
endif()

set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1) 

#--------------------------------------------------------------------------------------------------#
 
project(Service_Initialization) 

#--------------------------------------------------------------------------------------------------#

include_directories (
	../../02_Middleware/Library_StdLib/inc 
	./inc 
) 

#--------------------------------------------------------------------------------------------------#

add_executable(Service_Initialization
	./src/Service_Initialization.c
	./test/TI_Service_Initialization.c
)

#--------------------------------------------------------------------------------------------------# 

add_test(NAME Library_Gamepad COMMAND Library_Gamepad)

#--------------------------------------------------------------------------------------------------#

target_link_libraries(Library_Gamepad m  gcov) 

#--------------------------------------------------------------------------------------------------#

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(docInitialization
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

#--------------------------------------------------------------------------------------------------#
