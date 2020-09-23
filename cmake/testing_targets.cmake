# ====================================================
# Memory Tests
# ====================================================
add_executable(test_memory "${PROJECT_ROOT}/tests/memory/test_generic_utils.cpp")
target_link_libraries(test_memory PRIVATE
  # Public Includes
  gtest_inc
  chimera_inc
  aurora_inc
  prj_device_target

  # External Static Libraries
  gtest_src
  gtest_main
  chimera_src

  # Driver Under Test Libraries
  memory_generic
)


# cmake -Wno-dev -DTOOLCHAIN=gcc -DGTEST=ON -DNATIVE_THREADS=ON --configure ../