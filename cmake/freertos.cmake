###############################################################################
#                                Fetch freertos                               #
###############################################################################

function(freertos_support)
  include(FetchContent)
  cmake_parse_arguments(RTOS "" "DESTINATION" "EXTRA_LIBS" ${ARGN} )
  set(LIB_NAME "freertos")
  set(FREERTOS_DIR "${RTOS_DESTINATION}/FreeRtos")
  #set(FREERTOS_PORT GCC_ARM_CM0 CACHE STRING "")


  add_library(freertos_config INTERFACE)
  target_include_directories(freertos_config SYSTEM
      INTERFACE
      ${CMAKE_SOURCE_DIR}/config
    )

    target_compile_definitions(freertos_config
      INTERFACE
    )
    set(FREERTOS_PORT "GCC_ARM_CM0")
    set(FREERTOS_HEAP "3")

  FetchContent_Declare(FreeRtos_fetch
    GIT_REPOSITORY    https://github.com/FreeRTOS/FreeRTOS-Kernel.git
    GIT_TAG           fe005419f0d31264059c61a2f043a0b4e0f24076
    GIT_SHALLOW       True
    SOURCE_DIR        ${FREERTOS_DIR}
    CMAKE_ARGS        -DFREERTOS_HEAP=3 -DFREERTOS_PORT=GCC_ARM_CM0
    )

    FetchContent_MakeAvailable(FreeRtos_fetch)

    #add library




endfunction(freertos_support)
