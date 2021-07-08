include("${PROJECT_ROOT}/tests/aurora_test_harness.cmake")

# Tests as standalone executable
unit_test_executable(
  TARGET
    TestSharedPtr
  SOURCES
    basic_tests.cpp
  LIBRARIES
    aurora_tests_shared_entry
)

# Tests compiled into a library
gen_static_lib_variants(
  TARGET
    aurora_tests_shared_ptr
  SOURCES
    basic_tests.cpp
  PRV_LIBRARIES
    aurora_test_harness_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Tests"
)