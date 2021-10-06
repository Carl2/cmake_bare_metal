include(${CMAKE_SOURCE_DIR}/cmake/external_dep.cmake)
#f4_device_support(DESTINATION "${CMAKE_SOURCE_DIR}/external/cmsis_device_f4" )
f4_HAL_support(DESTINATION "${CMAKE_SOURCE_DIR}/external/" CONF_DIR "${CMAKE_SOURCE_DIR}/src/black_pill/Inc" )

add_library(startup OBJECT Startup/startup_stm32f411retx.s)



set(TARGET_BIN "BlackPill")

set(SRC
  Src/main.c
  #Startup/startup_stm32f411retx.s
  Src/stm32f4xx_hal_msp.c
  Src/stm32f4xx_it.c
  Src/system_stm32f4xx.c
  Src/syscalls.c
  Src/sysmem.c

  )

add_executable(${TARGET_BIN} ${SRC})

target_include_directories(${TARGET_BIN} PRIVATE Inc ${STM_DEVICE_SUPPORT_INCLUDE})

target_link_libraries(${TARGET_BIN} PRIVATE
  #project_sanitizer
  #CMSIS
  startup
  Stm32MCUSupport
  compile_flags
  compile_definitions
  )


add_custom_command(
  TARGET ${TARGET_BIN} POST_BUILD
  COMMAND arm-none-eabi-objcopy  -O ihex ${TARGET_BIN} ${TARGET_BIN}.hex
  VERBATIM
  )
