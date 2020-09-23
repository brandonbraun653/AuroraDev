cmake_minimum_required(VERSION 3.18.0)
project(AuroraDev VERSION 0.1.0)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# ====================================================
# Perform very high level configuration
# ====================================================
# Set some vars common to both development environments
include("${COMMON_TOOL_ROOT}/cmake/options/common.cmake")

# Override the linker command
if(Toolchain::HAS_GCC_ARM_NONE_EABI)
  include("${COMMON_TOOL_ROOT}/cmake/toolchains/linker_whole_archive.cmake")
endif()

# ====================================================
# Import common packages and projects
# ====================================================
find_package(Boost REQUIRED)
add_subdirectory("${PROJECT_ROOT}/lib/CommonTools/googletest")
add_subdirectory("${PROJECT_ROOT}/lib/Chimera")
add_subdirectory("${PROJECT_ROOT}/Aurora")

# ====================================================
# Build Targets
# ====================================================
include("${PROJECT_ROOT}/cmake/testing_targets.cmake")

# ====================================================
# System specific build options
# ====================================================
set(TARGET_CHIP prj_device_target)
add_library(${TARGET_CHIP} INTERFACE)
target_compile_options(${TARGET_CHIP} INTERFACE
  -fdata-sections
  -ffunction-sections
  -fmessage-length=0
  -fno-common
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
)

target_link_options(${TARGET_CHIP} INTERFACE
  -Wl,--gc-sections
)

target_compile_definitions(${TARGET_CHIP} INTERFACE
  CHIMERA_LITTLE_ENDIAN
)

export(TARGETS ${TARGET_CHIP} FILE "${PROJECT_BINARY_DIR}/DeviceTarget/${TARGET_CHIP}.cmake")