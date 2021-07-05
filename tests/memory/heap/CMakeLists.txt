# -----------------------------------------------------------------------------
# Test Suite:
#
# Without Map:
# cmake -DTOOLCHAIN=gcc -DNATIVE_THREADS=ON -DCMAKE_BUILD_TYPE=Debug --configure ../
# -----------------------------------------------------------------------------
include("${PROJECT_ROOT}/tests/aurora_test_harness.cmake")

unit_test_executable(
  TARGET
    TestHeap
  SOURCES
    main.cpp
    basic_allocation.cpp
)
