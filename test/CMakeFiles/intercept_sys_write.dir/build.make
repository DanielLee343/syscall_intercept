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
include test/CMakeFiles/intercept_sys_write.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/intercept_sys_write.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/intercept_sys_write.dir/flags.make

test/CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.o: test/CMakeFiles/intercept_sys_write.dir/flags.make
test/CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.o: test/intercept_sys_write.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cc/syscall_intercept/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.o"
	cd /home/cc/syscall_intercept/test && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.o   -c /home/cc/syscall_intercept/test/intercept_sys_write.c

test/CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.i"
	cd /home/cc/syscall_intercept/test && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cc/syscall_intercept/test/intercept_sys_write.c > CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.i

test/CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.s"
	cd /home/cc/syscall_intercept/test && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cc/syscall_intercept/test/intercept_sys_write.c -o CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.s

# Object files for target intercept_sys_write
intercept_sys_write_OBJECTS = \
"CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.o"

# External object files for target intercept_sys_write
intercept_sys_write_EXTERNAL_OBJECTS =

test/libintercept_sys_write.so: test/CMakeFiles/intercept_sys_write.dir/intercept_sys_write.c.o
test/libintercept_sys_write.so: test/CMakeFiles/intercept_sys_write.dir/build.make
test/libintercept_sys_write.so: libsyscall_intercept.so.0.1.0
test/libintercept_sys_write.so: test/CMakeFiles/intercept_sys_write.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cc/syscall_intercept/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libintercept_sys_write.so"
	cd /home/cc/syscall_intercept/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/intercept_sys_write.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/intercept_sys_write.dir/build: test/libintercept_sys_write.so

.PHONY : test/CMakeFiles/intercept_sys_write.dir/build

test/CMakeFiles/intercept_sys_write.dir/clean:
	cd /home/cc/syscall_intercept/test && $(CMAKE_COMMAND) -P CMakeFiles/intercept_sys_write.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/intercept_sys_write.dir/clean

test/CMakeFiles/intercept_sys_write.dir/depend:
	cd /home/cc/syscall_intercept && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cc/syscall_intercept /home/cc/syscall_intercept/test /home/cc/syscall_intercept /home/cc/syscall_intercept/test /home/cc/syscall_intercept/test/CMakeFiles/intercept_sys_write.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/intercept_sys_write.dir/depend

