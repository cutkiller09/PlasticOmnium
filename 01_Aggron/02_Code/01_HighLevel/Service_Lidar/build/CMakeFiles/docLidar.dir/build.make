# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar/build

# Utility rule file for docLidar.

# Include the progress variables for this target.
include CMakeFiles/docLidar.dir/progress.make

CMakeFiles/docLidar:
	$(CMAKE_COMMAND) -E cmake_progress_report /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating API documentation with Doxygen"
	/usr/bin/doxygen /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar/build/Doxyfile

docLidar: CMakeFiles/docLidar
docLidar: CMakeFiles/docLidar.dir/build.make
.PHONY : docLidar

# Rule to build all files generated by this target.
CMakeFiles/docLidar.dir/build: docLidar
.PHONY : CMakeFiles/docLidar.dir/build

CMakeFiles/docLidar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/docLidar.dir/cmake_clean.cmake
.PHONY : CMakeFiles/docLidar.dir/clean

CMakeFiles/docLidar.dir/depend:
	cd /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar/build /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar/build /home/seb/01_Project/07_Aggron/01_Aggron/02_Code/01_HighLevel/Service_Lidar/build/CMakeFiles/docLidar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/docLidar.dir/depend

