# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gong/projects/galay_co

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gong/projects/galay_co/build

# Include any dependencies generated for this target.
include src/CMakeFiles/galay_co.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/galay_co.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/galay_co.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/galay_co.dir/flags.make

src/CMakeFiles/galay_co.dir/galay_co.cc.o: src/CMakeFiles/galay_co.dir/flags.make
src/CMakeFiles/galay_co.dir/galay_co.cc.o: /home/gong/projects/galay_co/src/galay_co.cc
src/CMakeFiles/galay_co.dir/galay_co.cc.o: src/CMakeFiles/galay_co.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gong/projects/galay_co/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/galay_co.dir/galay_co.cc.o"
	cd /home/gong/projects/galay_co/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/galay_co.dir/galay_co.cc.o -MF CMakeFiles/galay_co.dir/galay_co.cc.o.d -o CMakeFiles/galay_co.dir/galay_co.cc.o -c /home/gong/projects/galay_co/src/galay_co.cc

src/CMakeFiles/galay_co.dir/galay_co.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/galay_co.dir/galay_co.cc.i"
	cd /home/gong/projects/galay_co/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gong/projects/galay_co/src/galay_co.cc > CMakeFiles/galay_co.dir/galay_co.cc.i

src/CMakeFiles/galay_co.dir/galay_co.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/galay_co.dir/galay_co.cc.s"
	cd /home/gong/projects/galay_co/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gong/projects/galay_co/src/galay_co.cc -o CMakeFiles/galay_co.dir/galay_co.cc.s

src/CMakeFiles/galay_co.dir/galay_co_scheduler.cc.o: src/CMakeFiles/galay_co.dir/flags.make
src/CMakeFiles/galay_co.dir/galay_co_scheduler.cc.o: /home/gong/projects/galay_co/src/galay_co_scheduler.cc
src/CMakeFiles/galay_co.dir/galay_co_scheduler.cc.o: src/CMakeFiles/galay_co.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gong/projects/galay_co/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/galay_co.dir/galay_co_scheduler.cc.o"
	cd /home/gong/projects/galay_co/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/galay_co.dir/galay_co_scheduler.cc.o -MF CMakeFiles/galay_co.dir/galay_co_scheduler.cc.o.d -o CMakeFiles/galay_co.dir/galay_co_scheduler.cc.o -c /home/gong/projects/galay_co/src/galay_co_scheduler.cc

src/CMakeFiles/galay_co.dir/galay_co_scheduler.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/galay_co.dir/galay_co_scheduler.cc.i"
	cd /home/gong/projects/galay_co/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gong/projects/galay_co/src/galay_co_scheduler.cc > CMakeFiles/galay_co.dir/galay_co_scheduler.cc.i

src/CMakeFiles/galay_co.dir/galay_co_scheduler.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/galay_co.dir/galay_co_scheduler.cc.s"
	cd /home/gong/projects/galay_co/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gong/projects/galay_co/src/galay_co_scheduler.cc -o CMakeFiles/galay_co.dir/galay_co_scheduler.cc.s

# Object files for target galay_co
galay_co_OBJECTS = \
"CMakeFiles/galay_co.dir/galay_co.cc.o" \
"CMakeFiles/galay_co.dir/galay_co_scheduler.cc.o"

# External object files for target galay_co
galay_co_EXTERNAL_OBJECTS =

src/libgalay_co.a: src/CMakeFiles/galay_co.dir/galay_co.cc.o
src/libgalay_co.a: src/CMakeFiles/galay_co.dir/galay_co_scheduler.cc.o
src/libgalay_co.a: src/CMakeFiles/galay_co.dir/build.make
src/libgalay_co.a: src/CMakeFiles/galay_co.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gong/projects/galay_co/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libgalay_co.a"
	cd /home/gong/projects/galay_co/build/src && $(CMAKE_COMMAND) -P CMakeFiles/galay_co.dir/cmake_clean_target.cmake
	cd /home/gong/projects/galay_co/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/galay_co.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/galay_co.dir/build: src/libgalay_co.a
.PHONY : src/CMakeFiles/galay_co.dir/build

src/CMakeFiles/galay_co.dir/clean:
	cd /home/gong/projects/galay_co/build/src && $(CMAKE_COMMAND) -P CMakeFiles/galay_co.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/galay_co.dir/clean

src/CMakeFiles/galay_co.dir/depend:
	cd /home/gong/projects/galay_co/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gong/projects/galay_co /home/gong/projects/galay_co/src /home/gong/projects/galay_co/build /home/gong/projects/galay_co/build/src /home/gong/projects/galay_co/build/src/CMakeFiles/galay_co.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/galay_co.dir/depend

