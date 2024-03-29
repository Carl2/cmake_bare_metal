###############################################################################
#                    Download and build the external CMSIS                    #
###############################################################################
include(${CMAKE_SOURCE_DIR}/cmake/external_dep.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/mcu_command_targets.cmake)

f4_HAL_support(DESTINATION "${CMAKE_SOURCE_DIR}/external/" CONFIG_DIR "${CMAKE_SOURCE_DIR}/src/Stepper/Inc" )


###############################################################################
#                       Create the startup object (ASM)                       #
###############################################################################
add_library(startup OBJECT Startup/startup_stm32f401ccux.s)


###############################################################################
#                            The actual execcutable                           #
###############################################################################
set(TARGET_BIN "Stepper")

set(SRC
  Src/main.c
  Src/stm32f4xx_hal_msp.c
  Src/stm32f4xx_it.c
  Src/system_stm32f4xx.c
  Src/syscalls.c
  Src/sysmem.c

  )

add_executable(${TARGET_BIN} ${SRC})

target_include_directories(${TARGET_BIN} PRIVATE Inc)

target_link_libraries(${TARGET_BIN} PRIVATE
  #project_sanitizer
  #  CMSIS
  startup
  stm32F4_HAL_support_lib
  compile_flags
  compile_definitions

  )


make_mcu_cmd_target( NAME ${TARGET_BIN} )
