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


  message(STATUS "Now ${PRJ_MCU} ")


endfunction(set_project_config )
