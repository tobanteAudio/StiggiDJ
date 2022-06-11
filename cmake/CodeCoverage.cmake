# Code Coverage Configuration
add_library(ta_coverage INTERFACE)
add_library(ta::coverage ALIAS ta_coverage)

if(TA_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ta_coverage INTERFACE -O0 -g --coverage)
  target_link_libraries(ta_coverage INTERFACE --coverage)
  target_link_options(ta_coverage INTERFACE --coverage)
endif()