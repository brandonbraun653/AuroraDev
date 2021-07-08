include("${PROJECT_ROOT}/tests/aurora_test_harness.cmake")
include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    aurora_tests_shared_entry
  SOURCES
    shared_main.cpp
  PRV_LIBRARIES
    aurora_intf_inc
    chimera_intf_inc
    gtest_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Tests"
)

unit_test_executable(
  TARGET
    TestAll
  SOURCES
    shared_main.cpp
  LIBRARIES
    aurora_tests_heap
    aurora_tests_shared_ptr
)
