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
include CMakeFiles/request_parser_lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/request_parser_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/request_parser_lib.dir/flags.make

CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o: CMakeFiles/request_parser_lib.dir/flags.make
CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o: ../src/request_parser.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/usr/src/projects/aaa-p2/build_coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o -c /usr/src/projects/aaa-p2/src/request_parser.cc

CMakeFiles/request_parser_lib.dir/src/request_parser.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_parser_lib.dir/src/request_parser.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/projects/aaa-p2/src/request_parser.cc > CMakeFiles/request_parser_lib.dir/src/request_parser.cc.i

CMakeFiles/request_parser_lib.dir/src/request_parser.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_parser_lib.dir/src/request_parser.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/projects/aaa-p2/src/request_parser.cc -o CMakeFiles/request_parser_lib.dir/src/request_parser.cc.s

CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o.requires:

.PHONY : CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o.requires

CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o.provides: CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o.requires
	$(MAKE) -f CMakeFiles/request_parser_lib.dir/build.make CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o.provides.build
.PHONY : CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o.provides

CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o.provides.build: CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o


# Object files for target request_parser_lib
request_parser_lib_OBJECTS = \
"CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o"

# External object files for target request_parser_lib
request_parser_lib_EXTERNAL_OBJECTS =

librequest_parser_lib.a: CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o
librequest_parser_lib.a: CMakeFiles/request_parser_lib.dir/build.make
librequest_parser_lib.a: CMakeFiles/request_parser_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/usr/src/projects/aaa-p2/build_coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library librequest_parser_lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/request_parser_lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/request_parser_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/request_parser_lib.dir/build: librequest_parser_lib.a

.PHONY : CMakeFiles/request_parser_lib.dir/build

CMakeFiles/request_parser_lib.dir/requires: CMakeFiles/request_parser_lib.dir/src/request_parser.cc.o.requires

.PHONY : CMakeFiles/request_parser_lib.dir/requires

CMakeFiles/request_parser_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/request_parser_lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/request_parser_lib.dir/clean

CMakeFiles/request_parser_lib.dir/depend:
	cd /usr/src/projects/aaa-p2/build_coverage && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /usr/src/projects/aaa-p2 /usr/src/projects/aaa-p2 /usr/src/projects/aaa-p2/build_coverage /usr/src/projects/aaa-p2/build_coverage /usr/src/projects/aaa-p2/build_coverage/CMakeFiles/request_parser_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/request_parser_lib.dir/depend

