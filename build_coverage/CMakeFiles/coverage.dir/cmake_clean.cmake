file(REMOVE_RECURSE
  "report"
  "CMakeFiles/coverage"
  "report/index.html"
  "coverage_data.stamp"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/coverage.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
