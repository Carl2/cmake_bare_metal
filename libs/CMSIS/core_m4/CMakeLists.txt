set(LIB_NAME "core")


add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE Include)

target_link_libraries(${LIB_NAME} INTERFACE
  compile_flags
  compile_definitions
  )
