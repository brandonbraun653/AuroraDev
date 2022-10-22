include("${PROJECT_ROOT}/tests/aurora_test_harness.cmake")

set(sourcefiles
  test_file_system.cpp
)

# Tests as standalone executable
unit_test_executable(
  TARGET
    TestFilesystem
  SOURCES
    ${sourcefiles}
  LIBRARIES
    aurora_tests_shared_entry
)

# Tests compiled into a library
gen_static_lib_variants(
  TARGET
    aurora_tests_filesystem
  SOURCES
    ${sourcefiles}
  PRV_LIBRARIES
    aurora_test_harness_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Tests"
)
