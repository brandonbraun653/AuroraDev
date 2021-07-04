# -----------------------------------------------------------------------------
# Test Suite: HLD ADC
#
# Without Map:
# cmake -DTOOLCHAIN=gcc -DNATIVE_THREADS=ON -DCMAKE_BUILD_TYPE=Debug --configure ../
# -----------------------------------------------------------------------------
add_executable(Test_PowerUp
  main.cpp
)

target_include_directories(Test_PowerUp PRIVATE "." )

target_link_libraries(Test_PowerUp PRIVATE
  # Public Includes
  aurora_intf_inc
  chimera_intf_inc

  # Utility Libraries
  aurora_core_dbg
  aurora_hmi_dbg
  aurora_logging_dbg
  aurora_memory_dbg
  aurora_database_dbg
  aurora_datastore_dbg
  aurora_filesystem_lfs_dbg

  # Hardware Libraries
  chimera_core_dbg
  chimera_sim_core_dbg

  # Target Properties
  prj_device_target
  prj_build_target
)


target_include_directories(Test_PowerUp PRIVATE ${PROJECT_ROOT})
add_custom_command(TARGET Test_PowerUp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Test_PowerUp> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sys_image
)
