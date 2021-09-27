add_library(startup OBJECT Startup/startup_stm32f411retx.s)
target_link_libraries(startup PUBLIC
  compile_flags
  compile_definitions
  )



set(TARGET_BIN "BlackPill")

set(SRC
  Src/main.c
  Startup/startup_stm32f411retx.s
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
#  startup
  Stm32MCUSupport
  compile_flags
  compile_definitions

  )


add_custom_command(
  TARGET ${TARGET_BIN} POST_BUILD
  COMMAND arm-none-eabi-objcopy  -O ihex ${TARGET_BIN} ${TARGET_BIN}.hex
  VERBATIM
  )
