# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /home/cc/syscall_intercept

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cc/syscall_intercept

# Utility rule file for check_whitespace.

# Include the progress variables for this target.
include CMakeFiles/check_whitespace.dir/progress.make

CMakeFiles/check_whitespace:
	/usr/bin/perl /home/cc/syscall_intercept/utils/check_whitespace.pl -r /home/cc/syscall_intercept/src

check_whitespace: CMakeFiles/check_whitespace
check_whitespace: CMakeFiles/check_whitespace.dir/build.make

.PHONY : check_whitespace

# Rule to build all files generated by this target.
CMakeFiles/check_whitespace.dir/build: check_whitespace

.PHONY : CMakeFiles/check_whitespace.dir/build

CMakeFiles/check_whitespace.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/check_whitespace.dir/cmake_clean.cmake
.PHONY : CMakeFiles/check_whitespace.dir/clean

CMakeFiles/check_whitespace.dir/depend:
	cd /home/cc/syscall_intercept && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cc/syscall_intercept /home/cc/syscall_intercept /home/cc/syscall_intercept /home/cc/syscall_intercept /home/cc/syscall_intercept/CMakeFiles/check_whitespace.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/check_whitespace.dir/depend
