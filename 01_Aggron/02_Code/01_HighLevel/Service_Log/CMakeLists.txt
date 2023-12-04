cmake_minimum_required(VERSION 2.8)

#--------------------------------------------------------------------------------------------------#
 
project(Service_Log) 

#--------------------------------------------------------------------------------------------------#

include_directories (
	../../02_Middleware/Library_StdLib/inc 
	./inc 
) 

#--------------------------------------------------------------------------------------------------#

add_executable(Service_Log
	./src/Service_Log.c
	./test/TI_Service_Log.c
)

#--------------------------------------------------------------------------------------------------#

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(docLog
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

#--------------------------------------------------------------------------------------------------#
