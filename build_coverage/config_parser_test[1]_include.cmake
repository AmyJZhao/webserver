if(EXISTS "/usr/src/projects/aaa-p2/build_coverage/config_parser_test[1]_tests.cmake")
  include("/usr/src/projects/aaa-p2/build_coverage/config_parser_test[1]_tests.cmake")
else()
  add_test(config_parser_test_NOT_BUILT config_parser_test_NOT_BUILT)
endif()
