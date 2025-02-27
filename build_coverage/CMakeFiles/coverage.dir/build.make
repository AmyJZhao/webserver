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

# Utility rule file for coverage.

# Include the progress variables for this target.
include CMakeFiles/coverage.dir/progress.make

CMakeFiles/coverage: report/index.html


report/index.html: coverage_data.stamp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/usr/src/projects/aaa-p2/build_coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating coverage report"
	/usr/bin/cmake -E remove_directory /usr/src/projects/aaa-p2/build_coverage/report
	/usr/bin/cmake -E make_directory /usr/src/projects/aaa-p2/build_coverage/report
	/usr/bin/gcovr --html --html-details -s -r /usr/src/projects/aaa-p2 --object-directory /usr/src/projects/aaa-p2/build_coverage -o /usr/src/projects/aaa-p2/build_coverage/report/index.html

coverage_data.stamp: bin/server
coverage_data.stamp: /usr/lib/x86_64-linux-gnu/libboost_system.a
coverage_data.stamp: libsession_lib.a
coverage_data.stamp: libserver_lib.a
coverage_data.stamp: libconfig_parser.a
coverage_data.stamp: librequest_handler_lib.a
coverage_data.stamp: bin/request_parser_test
coverage_data.stamp: bin/request_handler_test
coverage_data.stamp: bin/config_parser_test
coverage_data.stamp: bin/config_port_number_test
coverage_data.stamp: bin/server_test
coverage_data.stamp: bin/session_test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/usr/src/projects/aaa-p2/build_coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Running tests to generate coverage data"
	find /usr/src/projects/aaa-p2/build_coverage -name "*.gcda" -exec rm {} ";"
	/usr/bin/ctest
	touch /usr/src/projects/aaa-p2/build_coverage/coverage_data.stamp

coverage: CMakeFiles/coverage
coverage: report/index.html
coverage: coverage_data.stamp
coverage: CMakeFiles/coverage.dir/build.make

.PHONY : coverage

# Rule to build all files generated by this target.
CMakeFiles/coverage.dir/build: coverage

.PHONY : CMakeFiles/coverage.dir/build

CMakeFiles/coverage.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/coverage.dir/cmake_clean.cmake
.PHONY : CMakeFiles/coverage.dir/clean

CMakeFiles/coverage.dir/depend:
	cd /usr/src/projects/aaa-p2/build_coverage && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /usr/src/projects/aaa-p2 /usr/src/projects/aaa-p2 /usr/src/projects/aaa-p2/build_coverage /usr/src/projects/aaa-p2/build_coverage /usr/src/projects/aaa-p2/build_coverage/CMakeFiles/coverage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/coverage.dir/depend

