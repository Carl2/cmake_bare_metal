set(LIB_NAME "extra_conf")

add_library(${LIB_NAME} INTERFACE)
target_include_directories( ${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${LIB_NAME} INTERFACE
  compile_flags
  compile_definitions
  )
