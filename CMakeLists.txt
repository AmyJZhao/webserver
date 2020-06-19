cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(aaa-p2)
# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()
# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()
# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Enable cmake testing
include(CTest)
enable_testing()
# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)
# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system filesystem log regex) 
message(STATUS "Boost version: ${Boost_VERSION}")
# Get zlib
find_package(ZLIB)
include_directories(include ${ZLIB_INCLUDE_DIRS})
#############################################
# LIBRARY AND EXECUTABLE SETUP              #
#############################################
### To include std::filesystem, use: target_link_libraries(<executable> stdc++fs)
add_library(leaderboard src/leaderboard.cc)                         # Leaderboard library
add_library(request_lib src/request.cc)                             # Request library
add_library(response_helper src/response_helper.cc)                 # Response helper library
add_library(config_parser_lib src/config_parser.cc)                 # Config parser library
add_library(request_parser_lib src/request_parser.cc)               # Request parser library
add_library(server_info_lib src/server_information.cc)              # Server information library
add_library(logger_lib src/logger.cc)                               # Logger library
add_library(session_lib src/session.cc)                             # Session library
add_library(                                                        # Request handler library
    request_handler_lib 
    src/request_handler.cc 
    src/common_request_handler.cc 
    src/static_request_handler.cc 
    src/not_found_request_handler.cc 
    src/status_request_handler.cc 
    src/proxy_request_handler.cc
    src/health_request_handler.cc
    src/html_generator.cc
    src/snake_request_handler.cc
    src/leaderboard_request_handler.cc
)
target_link_libraries(
    request_handler_lib 
    request_lib 
    response_helper 
    server_info_lib 
    leaderboard
    ${ZLIB_LIBRARIES}
)
add_library(server_lib src/server.cc src/session.cc)                # Server library
target_link_libraries(
    server_lib
    session_lib  
    request_handler_lib 
    request_parser_lib 
    logger_lib 
    config_parser_lib
    Boost::system Boost::filesystem Boost::regex Boost::log_setup Boost::log
)
# Main executable and dependencies
add_executable(server src/server_main.cc)
target_link_libraries(server server_lib)
#############################################
# UNIT TESTS                                #
#############################################
# Leaderboard test
add_executable(leaderboard_test tests/unit/leaderboard_test.cc)
target_link_libraries(leaderboard_test leaderboard gtest_main)
gtest_discover_tests(leaderboard_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# Request parser test
# add_executable(request_parser_test tests/unit/request_parser_test.cc)
# target_link_libraries(request_parser_test request_parser_lib gtest_main)
# gtest_discover_tests(request_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# Request handler test
# add_executable(request_handler_test tests/unit/request_handler_test.cc)
# target_link_libraries(request_handler_test request_handler_lib Boost::system gtest_main)
# gtest_discover_tests(request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# Config parser test
add_executable(config_parser_test tests/unit/config_parser_test.cc)
target_link_libraries(config_parser_test config_parser_lib gtest_main)
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/config_parser_cases)
# Config parser semantics test
add_executable(config_parser_semantics_test tests/unit/config_parser_semantics_test.cc)
target_link_libraries(config_parser_semantics_test config_parser_lib gtest_main)
gtest_discover_tests(config_parser_semantics_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# Session test
# add_executable(session_test tests/unit/session_test.cc)
# target_link_libraries(session_test Boost::system session_lib request_handler_lib
#     request_parser_lib gtest_main logger_lib Boost::filesystem Boost::regex Boost::log_setup Boost::log)
# gtest_discover_tests(session_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# Server test
add_executable(server_test tests/unit/server_test.cc)
target_link_libraries(server_test server_lib server_info_lib session_lib Boost::system gtest_main)
gtest_discover_tests(server_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# Response test
add_executable(response_test tests/unit/response_test.cc)
target_link_libraries(response_test response_helper Boost::system gtest_main)
gtest_discover_tests(response_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# Logger test
add_executable(logger_test tests/unit/logger_test.cc)
target_link_libraries(logger_test logger_lib gtest_main Boost::log)
gtest_discover_tests(logger_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# Handler creation test
add_executable(handler_init_test tests/unit/request_handler_creation_test.cc)
target_link_libraries(handler_init_test config_parser_lib request_handler_lib logger_lib Boost::system Boost::filesystem Boost::log_setup Boost::log gtest_main)
gtest_discover_tests(handler_init_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# Proxy handler test
add_executable(proxy_handler_test tests/unit/proxy_handler_test.cc)
target_link_libraries(proxy_handler_test request_handler_lib config_parser_lib logger_lib gtest_main Boost::system Boost::filesystem Boost::log_setup Boost::log)
gtest_discover_tests(proxy_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

#############################################
# INTEGRATION TESTS                         #
#############################################
# Integration test
add_test(
    NAME integration_test 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/server_test.sh $<TARGET_FILE:server> ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/test_config
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration
)
add_custom_target(
    integration 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/server_test.sh $<TARGET_FILE:server> ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/test_config
)
# Proxy integration test
add_test(
    NAME proxy_integration_test 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/proxy_test.sh $<TARGET_FILE:server> ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/test_config ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/proxy_config
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration
)
add_custom_target(
    proxy_integration 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/proxy_test.sh $<TARGET_FILE:server> ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/test_config ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/proxy_config
)
# Multithreading Integration test
add_test(
    NAME multithreading_integration_test 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/multithreaded_test.sh $<TARGET_FILE:server> ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/multithreaded_config
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration
)
add_custom_target(
    multithreading_integration 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/multithreaded_test.sh $<TARGET_FILE:server> ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/multithreaded_config
)
# Snake integration test
add_test(
    NAME snake_integration_test 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/snake_test.sh $<TARGET_FILE:server> ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/snake_config
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)
add_custom_target(
    snake_integration 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/snake_test.sh $<TARGET_FILE:server> ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/snake_config
)
############################################
#CODE COVERAGE                             #
############################################
include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(
    TARGETS server Boost::system session_lib server_lib config_parser_lib request_handler_lib integration proxy_integration multithreading_integration snake_integration logger_lib
    TESTS handler_init_test config_parser_test config_parser_semantics_test response_test logger_test proxy_handler_test leaderboard_test server_test
)