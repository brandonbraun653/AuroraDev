# *****************************************************************************
# File Description:
#   Provides many targets useful for running tests on Aurora
#
# 2021 | Brandon Braun | brandonbraun653@gmail.com
# *****************************************************************************
include("${PROJECT_ROOT}/tests/aurora_test_harness.cmake")
include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")
include("${PROJECT_ROOT}/lib/CommonTools/cmake/modules/CodeCoverage.cmake")

# -----------------------------------------------------------------------------
# Builds main() for linking against individual tests
# -----------------------------------------------------------------------------
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

# -----------------------------------------------------------------------------
# Primary targets for Aurora testing:
#   TestAll
#   TestAllWithCoverage
#
# NOTE:
# If branched coverage isn't being generated, copy /etc/lcovrc to ~/.lcovrc and
# modify "lcov_branch_coverage" parameter to equal '1'.
# -----------------------------------------------------------------------------
unit_test_executable(
  TARGET
    TestAll
  SOURCES
    shared_main.cpp
  LIBRARIES
    aurora_tests_container
    aurora_tests_filesystem
    aurora_tests_heap
    aurora_tests_shared_ptr
)

# Create the coverage target
setup_target_for_coverage_lcov(
  NAME TestAllWithCoverage
  EXECUTABLE "${PROJECT_ROOT}/artifacts/last_build_version/TestAll"
  DEPENDENCIES TestAll
  BASE_DIRECTORY "${PROJECT_ROOT}"
  EXCLUDE "/usr/*" "${PROJECT_ROOT}/tests/*" "${PROJECT_ROOT}/lib/*" "${PROJECT_ROOT}/Aurora/lib/*"
  NO_DEMANGLE
)