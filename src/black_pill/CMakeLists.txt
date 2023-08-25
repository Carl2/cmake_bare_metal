###############################################################################
#                    Download and build the external CMSIS                    #
###############################################################################
include(${CMAKE_SOURCE_DIR}/cmake/external_dep.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/mcu_command_targets.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/freertos.cmake)

f4_HAL_support(DESTINATION "${CMAKE_SOURCE_DIR}/external/" CONFIG_DIR "${CMAKE_SOURCE_DIR}/src/black_pill/Inc" )


###############################################################################
#                       Create the startup object (ASM)                       #
###############################################################################
add_library(startup OBJECT Startup/startup_stm32f411retx.s)


###############################################################################
#                            The actual execcutable                           #
###############################################################################
set(TARGET_BIN "BlackPill")

set(SRC
  Src/main.c
  Src/stm32f4xx_hal_msp.c
  Src/stm32f4xx_it.c
  Src/system_stm32f4xx.c
  Src/syscalls.c
  Src/sysmem.c

  )

freertos_support(
 DESTINATION "${CMAKE_SOURCE_DIR}/external/"
 )

add_executable(${TARGET_BIN} ${SRC})


install(TARGETS ${TARGET_BIN}
  RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin)


target_link_libraries(${TARGET_BIN} PRIVATE
  startup
  stm32F4_HAL_support_lib
  compile_flags
  compile_definitions
  )


 make_mcu_cmd_target(NAME ${TARGET_BIN})

make_openocd_target(NAME ${TARGET_BIN}
    OPENOCD_PATH ${OPENOCD_PATH}
    OPENOCD_INTERFACE ${OPENOCD_INTERFACE}
    OPENOCD_TARGET stm32f4x

  )
