#------------------------------------------------------------------------------
# Auora Test Harness Build System Utilities
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.18.0)

#------------------------------------------------------------------------------
# FUNCTION: unit_test_executable
#
#   Generates an executable that will execute some kind of unit test. Intended
#   to help speed up development of tests by providing the common boilerplate
#   and libraries needed.
#
# INTERFACE:
#   TARGET
#       This is the name you want to reference the generated library by
#
#   SOURCES
#       Sources being used to create the executable
#
#   INCLUDES
#       Include paths
#
#   DEFINES
#       Compiler defintions
#
#   LIBRARIES
#       Libraries to build against
#
#------------------------------------------------------------------------------

function(unit_test_executable)
  #----------------------------------------------------------
  # Parse the input arguments
  #----------------------------------------------------------
  set(options "")
  set(oneValueArgs TARGET)
  set(multiValueArgs SOURCES INCLUDES DEFINES LIBRARIES)
  cmake_parse_arguments(UNIT_TEST_EXE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  #----------------------------------------------------------
  # Build the executable
  #----------------------------------------------------------
  add_executable(${UNIT_TEST_EXE_TARGET} ${UNIT_TEST_EXE_SOURCES})
  target_link_libraries(${UNIT_TEST_EXE_TARGET} PRIVATE aurora_test_harness_libs)

  if(UNIT_TEST_EXE_INCLUDES)
    target_include_directories(${UNIT_TEST_EXE_TARGET} PRIVATE ${UNIT_TEST_EXE_INCLUDES})
  endif()

  if(UNIT_TEST_EXE_DEFINES)
    target_compile_definitions(${UNIT_TEST_EXE_TARGET} PRIVATE ${UNIT_TEST_EXE_DEFINES})
  endif()

  if(UNIT_TEST_EXE_LIBRARIES)
    target_link_libraries(${UNIT_TEST_EXE_TARGET} PRIVATE ${UNIT_TEST_EXE_LIBRARIES})
  endif()

  # Export so other targets can use this
  export(TARGETS ${UNIT_TEST_EXE_TARGET} FILE "${PROJECT_BINARY_DIR}/Test/${UNIT_TEST_EXE_TARGET}.cmake")

  # Copy as explicit target name
  add_custom_command(
    TARGET ${UNIT_TEST_EXE_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${UNIT_TEST_EXE_TARGET}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${UNIT_TEST_EXE_TARGET}
    )

  # Copy as generic target for GDB debugging of last built image
  add_custom_command(
    TARGET ${UNIT_TEST_EXE_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${UNIT_TEST_EXE_TARGET}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/last_image
    )
endfunction()
