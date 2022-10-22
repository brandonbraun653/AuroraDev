cmake_minimum_required(VERSION 3.18.0)
project(AuroraDev VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CPP_PLATFORM "freertos")

# ====================================================
# Perform very high level configuration
# ====================================================
# Set some vars common to development environments
include("lib/CommonTools/cmake/options/common.cmake")

# Useful utilities for target generation
include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

# Override the linker command to allow out of order linking
if(Toolchain::HAS_GCC_ARM_NONE_EABI OR Toolchain::HAS_GCC)
  include("lib/CommonTools/cmake/toolchains/linker_whole_archive.cmake")
endif()

# ====================================================
# Import common packages and projects
# ====================================================
add_subdirectory("Aurora")
add_subdirectory("lib/Chimera")
add_subdirectory("lib/ChimeraSim")
add_subdirectory("lib/CommonTools/cpputest")
add_subdirectory("lib/CommonTools/googletest")
add_subdirectory("lib/CommonTools/cmake/device/generic")

# ====================================================
# Add test directory
# ====================================================
add_subdirectory("tests")
add_subdirectory("tests/container")
add_subdirectory("tests/filesystem")
add_subdirectory("tests/memory")

# ====================================================
# Common Targets for Testing
# ====================================================
# gen_intf_lib(
#   TARGET
#     project_inc
#   INTF_DEFINES
#     SIMULATOR
#   INTF_INCLUDES
#     "${PROJECT_ROOT}"
#   EXPORT_DIR
#     "${PROJECT_BINARY_DIR}/Project"
# )

gen_intf_lib(
  TARGET
    aurora_test_harness_inc
  INTF_LIBRARIES
    # Public Includes
    aurora_intf_inc
    chimera_intf_inc

    # Testing & Mock Libraries
    gmock_inc
    gtest_inc

    # Target Properties
    prj_device_target
    prj_build_target

  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Test"
)

gen_intf_lib(
  TARGET
    aurora_test_harness_libs
  INTF_LIBRARIES
    # Headers
    aurora_test_harness_inc

    # Utility Libraries
    aurora_core
    aurora_database
    aurora_datastore
    aurora_filesystem_core
    aurora_hmi
    aurora_logging
    aurora_memory

    # Hardware Libraries
    chimera_core
    chimera_sim_core

    # Testing & Mock Libraries
    gmock_src
    gtest_src

    # Simulator networking
    zmq

  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Test"
)
