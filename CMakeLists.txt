cmake_minimum_required(VERSION 3.18.0)
project(AuroraDev VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CPP_PLATFORM "freertos")

# ====================================================
# Perform very high level configuration
# ====================================================
# Set some vars common to both development environments
include("lib/CommonTools/cmake/options/common.cmake")

# ====================================================
# Import common packages and projects
# ====================================================
find_package(Boost REQUIRED)
add_subdirectory("Aurora")
add_subdirectory("lib/Chimera")
add_subdirectory("lib/ChimeraSim")
add_subdirectory("lib/CommonTools/cpputest")
add_subdirectory("lib/CommonTools/googletest")
add_subdirectory("lib/CommonTools/cmake/device/generic")

# ====================================================
# Add test directory
# ====================================================
add_subdirectory("tests/power_up")
