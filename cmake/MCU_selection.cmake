###############################################################################
#                    The idea here is to set the different
#                    Variables according to the project, and MCU
###############################################################################


function(set_project_config)

  set(options OPTIONAL )
  set(oneValueArgs MCU CONF)
  set(multiValueArgs )
  cmake_parse_arguments(PRJ "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  message(STATUS "Now ${PRJ_MCU} conf ${PRJ_CONF} ")

  file(GLOB prj_config_files
    "${CMAKE_SOURCE_DIR}/cmake/mcu_conf/*.cmake"
    )

  set(MCU_LIST "")
  foreach(mcu_file IN LISTS prj_config_files )
    string(REPLACE "${CMAKE_SOURCE_DIR}/cmake/mcu_conf/" "" mcu ${mcu_file})
    #string(REGEX MATCH "${CMAKE_SOURCE_DIR}/cmake/mcu_conf/*.cmake" mcu ${mcu_file})
    list(APPEND MCU_LIST ${mcu})
  endforeach()

  list(GET MCU_LIST 0 MCU_CONF_ONE)

  set(MCU_CONFIG ${MCU_CONF_ONE} CACHE STRING "Mcu config")
  set_property(CACHE MCU_CONFIG PROPERTY STRINGS ${MCU_LIST} )

endfunction(set_project_config )
