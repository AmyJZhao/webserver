# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /usr/src/projects/aaa-p2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /usr/src/projects/aaa-p2/build_coverage

# Include any dependencies generated for this target.
include googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/depend.make

# Include the progress variables for this target.
include googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/progress.make

# Include the compile flags for this target's objects.
include googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/flags.make

googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o: googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/flags.make
googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o: /usr/src/googletest/googletest/src/gtest_main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/src/projects/aaa-p2/build_coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o"
	cd /usr/src/projects/aaa-p2/build_coverage/googletest/googlemock/gtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gtest_main.dir/src/gtest_main.cc.o -c /usr/src/googletest/googletest/src/gtest_main.cc

googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gtest_main.dir/src/gtest_main.cc.i"
	cd /usr/src/projects/aaa-p2/build_coverage/googletest/googlemock/gtest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/googletest/googletest/src/gtest_main.cc > CMakeFiles/gtest_main.dir/src/gtest_main.cc.i

googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gtest_main.dir/src/gtest_main.cc.s"
	cd /usr/src/projects/aaa-p2/build_coverage/googletest/googlemock/gtest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/googletest/googletest/src/gtest_main.cc -o CMakeFiles/gtest_main.dir/src/gtest_main.cc.s

googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.requires:

.PHONY : googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.requires

googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.provides: googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.requires
	$(MAKE) -f googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/build.make googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.provides.build
.PHONY : googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.provides

googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.provides.build: googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o


# Object files for target gtest_main
gtest_main_OBJECTS = \
"CMakeFiles/gtest_main.dir/src/gtest_main.cc.o"

# External object files for target gtest_main
gtest_main_EXTERNAL_OBJECTS =

googletest/googlemock/gtest/libgtest_main.a: googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
googletest/googlemock/gtest/libgtest_main.a: googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/build.make
googletest/googlemock/gtest/libgtest_main.a: googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/usr/src/projects/aaa-p2/build_coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libgtest_main.a"
	cd /usr/src/projects/aaa-p2/build_coverage/googletest/googlemock/gtest && $(CMAKE_COMMAND) -P CMakeFiles/gtest_main.dir/cmake_clean_target.cmake
	cd /usr/src/projects/aaa-p2/build_coverage/googletest/googlemock/gtest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gtest_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/build: googletest/googlemock/gtest/libgtest_main.a

.PHONY : googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/build

googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/requires: googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.requires

.PHONY : googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/requires

googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/clean:
	cd /usr/src/projects/aaa-p2/build_coverage/googletest/googlemock/gtest && $(CMAKE_COMMAND) -P CMakeFiles/gtest_main.dir/cmake_clean.cmake
.PHONY : googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/clean

googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/depend:
	cd /usr/src/projects/aaa-p2/build_coverage && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /usr/src/projects/aaa-p2 /usr/src/googletest/googletest /usr/src/projects/aaa-p2/build_coverage /usr/src/projects/aaa-p2/build_coverage/googletest/googlemock/gtest /usr/src/projects/aaa-p2/build_coverage/googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : googletest/googlemock/gtest/CMakeFiles/gtest_main.dir/depend

