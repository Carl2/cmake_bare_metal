###############################################################################
#                    Download and build the external CMSIS                    #
###############################################################################
include(${CMAKE_SOURCE_DIR}/cmake/external_dep.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/mcu_command_targets.cmake)

f4_HAL_support(DESTINATION "${CMAKE_SOURCE_DIR}/external/" CONFIG_DIR "${CMAKE_SOURCE_DIR}/src/sml_test/Inc" )


###############################################################################
#                       Create the startup object (ASM)                       #
###############################################################################
add_library(startup OBJECT Startup/startup_stm32f401ccux.s)


###############################################################################
#                            Createa a c++ library                            #
###############################################################################
add_library(sm OBJECT cpp/statemachine_simple.cpp )
target_include_directories(sm PUBLIC  ${CMAKE_SOURCE_DIR}/external/sml/include)
target_link_libraries(sm PRIVATE
  stm32F4_HAL_support_lib
  compile_flags
  compile_definitions
  )

###############################################################################
#                            The actual execcutable                           #
###############################################################################
set(TARGET_BIN "sml_test")


# These files needs to be compiled with c++
set(SRC_CPP
  "Src/stm32f4xx_hal_msp.c"
  "Src/stm32f4xx_it.c"
  "Src/system_stm32f4xx.c"

  )

set(SRC
  ${SRC_CPP}
  Src/main.cpp
  Src/syscalls.c
  Src/sysmem.c
  )




add_executable(${TARGET_BIN} ${SRC})


set_source_files_properties(
  ${SRC_CPP}
  PROPERTIES
  LANGUAGE CXX
  )


target_include_directories(${TARGET_BIN} PRIVATE
  Inc
  cpp
  ${CMAKE_SOURCE_DIR}/external/sml/include

  )

target_link_libraries(${TARGET_BIN} PRIVATE
  #project_sanitizer
  #  CMSIS
  sm
  startup
  stm32F4_HAL_support_lib
  compile_flags
  compile_definitions

  )



make_mcu_cmd_target( NAME ${TARGET_BIN} )
