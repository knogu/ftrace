# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/jp31281/.cache/JetBrains/RemoteDev/dist/5e0b8d8cdf7c1_CLion-2023.3.1/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/jp31281/.cache/JetBrains/RemoteDev/dist/5e0b8d8cdf7c1_CLion-2023.3.1/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jp31281/ftrace

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jp31281/ftrace/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ftrace.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ftrace.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ftrace.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ftrace.dir/flags.make

CMakeFiles/ftrace.dir/hello.c.o: CMakeFiles/ftrace.dir/flags.make
CMakeFiles/ftrace.dir/hello.c.o: /home/jp31281/ftrace/hello.c
CMakeFiles/ftrace.dir/hello.c.o: CMakeFiles/ftrace.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jp31281/ftrace/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ftrace.dir/hello.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/ftrace.dir/hello.c.o -MF CMakeFiles/ftrace.dir/hello.c.o.d -o CMakeFiles/ftrace.dir/hello.c.o -c /home/jp31281/ftrace/hello.c

CMakeFiles/ftrace.dir/hello.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ftrace.dir/hello.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jp31281/ftrace/hello.c > CMakeFiles/ftrace.dir/hello.c.i

CMakeFiles/ftrace.dir/hello.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ftrace.dir/hello.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jp31281/ftrace/hello.c -o CMakeFiles/ftrace.dir/hello.c.s

CMakeFiles/ftrace.dir/hook.c.o: CMakeFiles/ftrace.dir/flags.make
CMakeFiles/ftrace.dir/hook.c.o: /home/jp31281/ftrace/hook.c
CMakeFiles/ftrace.dir/hook.c.o: CMakeFiles/ftrace.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jp31281/ftrace/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ftrace.dir/hook.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/ftrace.dir/hook.c.o -MF CMakeFiles/ftrace.dir/hook.c.o.d -o CMakeFiles/ftrace.dir/hook.c.o -c /home/jp31281/ftrace/hook.c

CMakeFiles/ftrace.dir/hook.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ftrace.dir/hook.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jp31281/ftrace/hook.c > CMakeFiles/ftrace.dir/hook.c.i

CMakeFiles/ftrace.dir/hook.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ftrace.dir/hook.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jp31281/ftrace/hook.c -o CMakeFiles/ftrace.dir/hook.c.s

# Object files for target ftrace
ftrace_OBJECTS = \
"CMakeFiles/ftrace.dir/hello.c.o" \
"CMakeFiles/ftrace.dir/hook.c.o"

# External object files for target ftrace
ftrace_EXTERNAL_OBJECTS =

ftrace: CMakeFiles/ftrace.dir/hello.c.o
ftrace: CMakeFiles/ftrace.dir/hook.c.o
ftrace: CMakeFiles/ftrace.dir/build.make
ftrace: CMakeFiles/ftrace.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jp31281/ftrace/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ftrace"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ftrace.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ftrace.dir/build: ftrace
.PHONY : CMakeFiles/ftrace.dir/build

CMakeFiles/ftrace.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ftrace.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ftrace.dir/clean

CMakeFiles/ftrace.dir/depend:
	cd /home/jp31281/ftrace/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jp31281/ftrace /home/jp31281/ftrace /home/jp31281/ftrace/cmake-build-debug /home/jp31281/ftrace/cmake-build-debug /home/jp31281/ftrace/cmake-build-debug/CMakeFiles/ftrace.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ftrace.dir/depend

