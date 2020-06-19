# CMake generated Testfile for 
# Source directory: /usr/src/projects/aaa-p2
# Build directory: /usr/src/projects/aaa-p2/build_coverage
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/usr/src/projects/aaa-p2/build_coverage/request_parser_test[1]_include.cmake")
include("/usr/src/projects/aaa-p2/build_coverage/request_handler_test[1]_include.cmake")
include("/usr/src/projects/aaa-p2/build_coverage/config_parser_test[1]_include.cmake")
include("/usr/src/projects/aaa-p2/build_coverage/config_port_number_test[1]_include.cmake")
include("/usr/src/projects/aaa-p2/build_coverage/session_test[1]_include.cmake")
include("/usr/src/projects/aaa-p2/build_coverage/server_test[1]_include.cmake")
add_test(integration_test "/usr/src/projects/aaa-p2/tests/integration/server_test.sh" "/usr/src/projects/aaa-p2/build_coverage/bin/server" "/usr/src/projects/aaa-p2/tests/integration/test_config")
set_tests_properties(integration_test PROPERTIES  WORKING_DIRECTORY "/usr/src/projects/aaa-p2/tests/integration")
subdirs("googletest")
