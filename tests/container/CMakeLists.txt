include("${PROJECT_ROOT}/tests/aurora_test_harness.cmake")

set(sourcefiles
  test_circular_buffer.cpp
  test_mpmc_queue.cpp
)

# Tests as standalone executable
unit_test_executable(
  TARGET
    TestContainer
  SOURCES
    ${sourcefiles}
  LIBRARIES
    aurora_tests_shared_entry
)

# Tests compiled into a library
gen_static_lib_variants(
  TARGET
    aurora_tests_container
  SOURCES
    ${sourcefiles}
  PRV_LIBRARIES
    aurora_test_harness_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Tests"
)