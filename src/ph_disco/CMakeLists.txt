###############################################################################
#                    Download and build the external CMSIS                    #
###############################################################################
include(${CMAKE_SOURCE_DIR}/cmake/external_dep.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/mcu_command_targets.cmake)

f1_HAL_support(DESTINATION "${CMAKE_SOURCE_DIR}/external" CONFIG_DIR "${CMAKE_SOURCE_DIR}/src/ph_disco/Inc" )

###############################################################################
#                       Create the startup object (ASM)                       #
###############################################################################
add_library(startup OBJECT Startup/startup_stm32f103t8ux.s)


###############################################################################
#                            The actual execcutable                           #
###############################################################################
set(TARGET_BIN "ph_disco")

set(SRC
  Src/main.c
  Src/stm32f1xx_it.c
  Src/stm32f1xx_hal_msp.c
  Src/system_stm32f1xx.c
  Src/syscalls.c
  Src/sysmem.c


  )

add_executable(${TARGET_BIN} ${SRC})

target_include_directories(${TARGET_BIN} PRIVATE Inc )

target_link_libraries(${TARGET_BIN} PRIVATE
  startup
  stm32F1_HAL_support_lib
  compile_flags
  compile_definitions
  )


add_custom_command(
  TARGET ${TARGET_BIN} POST_BUILD
  COMMAND arm-none-eabi-objcopy  -O ihex ${TARGET_BIN} ${TARGET_BIN}.hex
  VERBATIM
  )

make_mcu_cmd_target( NAME ${TARGET_BIN} )
