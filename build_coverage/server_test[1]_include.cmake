if(EXISTS "/usr/src/projects/aaa-p2/build_coverage/server_test[1]_tests.cmake")
  include("/usr/src/projects/aaa-p2/build_coverage/server_test[1]_tests.cmake")
else()
  add_test(server_test_NOT_BUILT server_test_NOT_BUILT)
endif()
