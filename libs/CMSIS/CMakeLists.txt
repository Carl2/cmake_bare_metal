add_subdirectory(core_m4)

set(LIB_NAME "CMSIS")

add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE STM32F4xx/Include)

target_link_libraries(${LIB_NAME} INTERFACE
  compile_flags
  compile_definitions
  core
  linker_flags
  )
