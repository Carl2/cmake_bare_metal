#TODO: Fix this
include_directories(include/common)
if(CONFIG STREQUAL "CONF_ONE")
  message(STATUS "Using Conf One")
  include_directories("include/conf_one")
else()
    message(STATUS "Using Conf Two")
  include_directories("include/conf_two")
endif()




if( FLAVOR STREQUAL "BLACK_PILL")
  message(STATUS "Black Pill is used")
  add_subdirectory(black_pill)
elseif(FLAVOR STREQUAL "BLACK_FREERTOS")
  message(STATUS "Black Pill with freertos is used")
  add_subdirectory(black_freertos)
elseif( FLAVOR STREQUAL "STEPPER")
  add_subdirectory(Stepper)
elseif( FLAVOR STREQUAL "SML_TEST")
  add_subdirectory(sml_test)
elseif( FLAVOR STREQUAL "PH_DISCO")
  add_subdirectory("ph_disco")
endif()
