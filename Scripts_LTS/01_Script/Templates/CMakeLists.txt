cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")

if(WIN32)
   set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
   set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
endif()

set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)

# Options
option(COVERAGE "Enable code coverage" ON)

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

if (COVERAGE)
    add_definitions(--coverage)
endif()

#--------------------------------------------------------------------------------------------------#
 
project($COMPONENT) 

#--------------------------------------------------------------------------------------------------#
set (SRCS  
	./src/$COMPONENT.c	 
)

include_directories ( 
	./inc 
	../../02_CORE/Base/inc
) 


set (TI_SRCS 
	#./test/02_SWE5_Integration_Tests/stubs/TBC.c #Complete with the good stubs
	./test/02_SWE5_Integration_Tests/src/TI_$COMPONENT.c
	./test/02_SWE5_Integration_Tests/src/TI_Report.c
)        
 
#-- if(NOT TARGET Lib_$COMPONENT) 					--#
#--	add_library(Lib_$COMPONENT STATIC ${SRCS} )  	--#
#-- endif()     									--#
   
#--------------------------------------------------------------------------------------------------#
# Ctests enabled
#--------------------------------------------------------------------------------------------------#
enable_testing()

add_test(
  NAME TI_$COMPONENT
  COMMAND $<TARGET_FILE:TI_$COMPONENT>
)

#--------------------------------------------------------------------------------------------------#

add_executable(TI_$COMPONENT ${TI_SRCS} ${SRCS} )   
target_link_libraries(TI_$COMPONENT --coverage)  
set_target_properties(TI_$COMPONENT PROPERTIES EXCLUDE_FROM_ALL TRUE) 


target_include_directories (TI_$COMPONENT PRIVATE
	#../../01_COM/Service_TBC/inc	# OPTIONNAL & Complete with the good name
	#../../02_CORE/Library_TBC/inc # OPTIONNAL & with the good name
	#../../03_DRV/Driver_TBC/inc 		# OPTIONNAL & with the good name
	./test/02_SWE5_Integration_Tests/stubs/inc
	./test/02_SWE5_Integration_Tests/inc
	./inc
)   
 
#--------------------------------------------------------------------------------------------------#

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc$COMPONENT
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

#--------------------------------------------------------------------------------------------------#
