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

# Include any dependencies generated for this target.
include test/CMakeFiles/executable_with_syscall_no_pie.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/executable_with_syscall_no_pie.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/executable_with_syscall_no_pie.dir/flags.make

test/CMakeFiles/executable_with_syscall_no_pie.dir/executable_with_syscall.S.o: test/CMakeFiles/executable_with_syscall_no_pie.dir/flags.make
test/CMakeFiles/executable_with_syscall_no_pie.dir/executable_with_syscall.S.o: test/executable_with_syscall.S
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cc/syscall_intercept/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ASM object test/CMakeFiles/executable_with_syscall_no_pie.dir/executable_with_syscall.S.o"
	cd /home/cc/syscall_intercept/test && /usr/bin/clang $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles/executable_with_syscall_no_pie.dir/executable_with_syscall.S.o -c /home/cc/syscall_intercept/test/executable_with_syscall.S

# Object files for target executable_with_syscall_no_pie
executable_with_syscall_no_pie_OBJECTS = \
"CMakeFiles/executable_with_syscall_no_pie.dir/executable_with_syscall.S.o"

# External object files for target executable_with_syscall_no_pie
executable_with_syscall_no_pie_EXTERNAL_OBJECTS =

test/executable_with_syscall_no_pie: test/CMakeFiles/executable_with_syscall_no_pie.dir/executable_with_syscall.S.o
test/executable_with_syscall_no_pie: test/CMakeFiles/executable_with_syscall_no_pie.dir/build.make
test/executable_with_syscall_no_pie: test/CMakeFiles/executable_with_syscall_no_pie.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cc/syscall_intercept/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking ASM executable executable_with_syscall_no_pie"
	cd /home/cc/syscall_intercept/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/executable_with_syscall_no_pie.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/executable_with_syscall_no_pie.dir/build: test/executable_with_syscall_no_pie

.PHONY : test/CMakeFiles/executable_with_syscall_no_pie.dir/build

test/CMakeFiles/executable_with_syscall_no_pie.dir/clean:
	cd /home/cc/syscall_intercept/test && $(CMAKE_COMMAND) -P CMakeFiles/executable_with_syscall_no_pie.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/executable_with_syscall_no_pie.dir/clean

test/CMakeFiles/executable_with_syscall_no_pie.dir/depend:
	cd /home/cc/syscall_intercept && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cc/syscall_intercept /home/cc/syscall_intercept/test /home/cc/syscall_intercept /home/cc/syscall_intercept/test /home/cc/syscall_intercept/test/CMakeFiles/executable_with_syscall_no_pie.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/executable_with_syscall_no_pie.dir/depend

