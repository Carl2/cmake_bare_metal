###############################################################################
#                                Fetch freertos                               #
###############################################################################

function(freertos_support)
  include(FetchContent)
  cmake_parse_arguments(RTOS "" "DESTINATION;CONFIG;PORT" "EXTRA_LIBS" ${ARGN} )

  set(FREERTOS_DIR "${RTOS_DESTINATION}/FreeRtos")

  message(STATUS "FreeRtos Config: ${RTOS_CONFIG} Port: ${RTOS_PORT}")
  add_library(freertos_config INTERFACE)
  target_include_directories(freertos_config SYSTEM
      INTERFACE
      ${RTOS_CONFIG}
    )

    target_compile_definitions(freertos_config
      INTERFACE
    )
    #set(FREERTOS_PORT "GCC_ARM_CM4F")
    #set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")
    set(FREERTOS_PORT ${RTOS_PORT})
    set(FREERTOS_HEAP "4")

  FetchContent_Declare(FreeRtos_fetch
    GIT_REPOSITORY    https://github.com/FreeRTOS/FreeRTOS-Kernel.git
    GIT_TAG           fe005419f0d31264059c61a2f043a0b4e0f24076
    GIT_SHALLOW       True

    SOURCE_DIR        ${FREERTOS_DIR}
    #CMAKE_ARGS        -DFREERTOS_HEAP=4
    )

    FetchContent_MakeAvailable(FreeRtos_fetch)

    #Make sure the freertos is built with the right flags.
    target_link_libraries(freertos_kernel_port PRIVATE compile_flags compile_definitions)
    target_link_libraries(freertos_kernel PRIVATE compile_flags compile_definitions)


endfunction(freertos_support)
