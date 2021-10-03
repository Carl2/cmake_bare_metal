###############################################################################
#                    The idea here is to set the different
#                    Variables according to the project, and MCU
###############################################################################


function(set_project_config)

  set(options OPTIONAL )
  set(oneValueArgs MCU CONF)
  set(multiValueArgs )
    cmake_parse_arguments(PRJ "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN} )


  message(STATUS "Now ${PRJ_MCU} conf ${PRJ_CONF} ")


  file(GLOB prj_config_files
    "${CMAKE_SOURCE_DIR}/cmake/mcu_conf/*.cmake"
    )

  message(STATUS "Configure files to use: ${prj_config_files}")
  list(GET prj_config_files 0 MCU_CONF_ONE)

  set(MCU_CONFIG ${MCU_CONF_ONE} CACHE STRING "Mcu config")
  set_property(CACHE MCU_CONFIG PROPERTY STRINGS ${prj_config_files} )





endfunction(set_project_config )
