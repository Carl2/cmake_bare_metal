if( CONFIG STREQUAL "CONF_TWO" )
  set(CPP_DIR "native")
else()
  set(CPP_DIR "stm32")
endif()


add_library(hal  src/${CPP_DIR}/hal.cpp)

target_include_directories(hal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hal
  compile_flags
  # project_options
  # project_warnings
  # project_sanitizer
  )
