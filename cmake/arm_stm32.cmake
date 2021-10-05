###############################################################################
#                     Compiler flags for different builds types               #
###############################################################################
# Debug


# Generate compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS On)


###############################################################################
#                                 M4 compiler flags
# arm-none-eabi-g++ -o "BlackPill_First.elf" @"objects.list"   \
# -mcpu=cortex-m4 -T"/home/calle/STM32CubeIDE/workspace_1.7.0/BlackPill_First/STM32F411RETX_FLASH.ld" \
# --specs=nosys.specs -Wl,-Map="BlackPill_First.map"
# -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group \
# -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
###############################################################################
function(set_stm32_m4_compiler_options project_name)
  # Optimize for debugging experience rather than speed or size his
  # results in considerably smaller code than the current -O0 optimization when DEBUG is set
  message(STATUS "Using ${CMAKE_CXX_COMPILER} ${CMAKE_C_COMPILER} ${CMAKE_BUILD_TYPE}" )


  target_compile_options(${project_name}
    INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:-mcpu=${MCU_COMPILER_TYPE}>
    $<$<COMPILE_LANGUAGE:C>:-mcpu=${MCU_COMPILER_TYPE}>
    $<$<COMPILE_LANGUAGE:ASM>:-mcpu=${MCU_COMPILER_TYPE}>

    $<$<COMPILE_LANGUAGE:CXX>:${MCU_COMPILER_INST_SET}>
    $<$<COMPILE_LANGUAGE:C>:${MCU_COMPILER_INST_SET}>
    $<$<COMPILE_LANGUAGE:ASM>:${MCU_COMPILER_INST_SET}>

    $<$<COMPILE_LANGUAGE:CXX>:-fmessage-length=0>
    $<$<COMPILE_LANGUAGE:C>:-fmessage-length=0>
    ###########################################################################
    #                                  Debug                                  #
    ###########################################################################
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-O0>
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-g3>

    $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:-O0>
    $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:-g3>
    $<$<AND:$<COMPILE_LANGUAGE:ASM>,$<CONFIG:Debug>>:-g>
    ###########################################################################
    #                                 Release                                 #
    ###########################################################################
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release>>:-O2>
    $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Release>>:-O2>
    $<$<AND:$<COMPILE_LANGUAGE:ASM>,$<CONFIG:Release>>:-warn>

    ###########################################################################
    #                                 warnings                                #
    ###########################################################################
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
  )

if(MCU_FLOAT)
    message(STATUS "FPU supported")
    target_compile_options(${project_name}
      INTERFACE
      $<$<COMPILE_LANGUAGE:CXX>:-mfloat-abi=hard>
      $<$<COMPILE_LANGUAGE:CXX>:-mfpu=${MCU_COMPILER_FPU_DEV}>
      $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
      $<$<COMPILE_LANGUAGE:C>:-mfpu=${MCU_COMPILER_FPU_DEV}>
      $<$<COMPILE_LANGUAGE:ASM>:-mfloat-abi=hard>
      $<$<COMPILE_LANGUAGE:ASM>:-mfpu=${MCU_COMPILER_FPU_DEV}>
      )
  else()
    target_compile_options(${project_name}
      INTERFACE
      $<$<COMPILE_LANGUAGE:CXX>:-mfloat-abi=soft>
      $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=soft>
      $<$<COMPILE_LANGUAGE:ASM>:-mfloat-abi=soft>
      )
endif(MCU_FLOAT)


  #############################################################################
  #                         Flags dependent on language                       #
  #############################################################################
  target_compile_options(${project_name} INTERFACE

    $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-pedantic-errors>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    )

endfunction()


function(set_stm32_m4_compiler_definition project_name)
  #STM32F411xE
  target_compile_definitions(${project_name} INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:DEBUG>
    $<$<COMPILE_LANGUAGE:C>:DEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:USE_FULL_ASSERT>
    $<$<COMPILE_LANGUAGE:C>:USE_FULL_ASSERT>
    $<$<COMPILE_LANGUAGE:CXX>:${MCU_DEF_MCU}>
    $<$<COMPILE_LANGUAGE:C>:${MCU_DEF_MCU}>
    $<$<COMPILE_LANGUAGE:CXX>:${MCU_DEF_HAL_DRIVER}>
    $<$<COMPILE_LANGUAGE:C>:${MCU_DEF_HAL_DRIVER}>
    )


endfunction()


function(set_stm32_m4_linker_flags project_name)

  # Set if FPU is defined , then we need to add that to the linker.
  if(MCU_FLOAT)
    set(STM_LINK_FLAGS
      -mfloat-abi=hard -mfpu=${MCU_COMPILER_FPU_DEV}
      )
  endif()



  set(STM_LINK_FLAGS
    ${STM_LINK_FLAGS}
    -L${MCU_MEMORY_LAYOUT_DIR}
    -T${MCU_MEMORY_LAYOUT_FILES}
    -mcpu=${MCU_COMPILER_TYPE}
    -mthumb
    -Xlinker
    -Map=${MCU_LINKER_MAP_FILE}
    -Wl,--gc-section
    -specs=${MCU_LINKER_SPEC}
    -Wl,--start-group # Searches for symbols several times in the libs
    -lc               # betweek start/end group
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group

    )

  target_link_options(${project_name} INTERFACE ${STM_LINK_FLAGS}   )


endfunction()















###############################################################################
#               Set the compiler options when used as interface               #
###############################################################################

function(set_stm32_m3_compiler_options project_name)
  # Optimize for debugging experience rather than speed or size his
  # results in considerably smaller code than the current -O0 optimization when DEBUG is set
  message(STATUS "Using ${CMAKE_CXX_COMPILER} ${CMAKE_C_COMPILER} ${CMAKE_BUILD_TYPE}" )


  target_compile_options(${project_name}
    INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:-mcpu=cortex-m3>
    $<$<COMPILE_LANGUAGE:C>:-mcpu=cortex-m3>
    $<$<COMPILE_LANGUAGE:ASM>:-mcpu=cortex-m3>

    $<$<COMPILE_LANGUAGE:CXX>:-mthumb>
    $<$<COMPILE_LANGUAGE:C>:-mthumb>
    $<$<COMPILE_LANGUAGE:ASM>:-mthumb>

    $<$<COMPILE_LANGUAGE:CXX>:-fmessage-length=0>
    $<$<COMPILE_LANGUAGE:C>:-fmessage-length=0>

    ###########################################################################
    #                                  Debug                                  #
    ###########################################################################
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-O0>
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-g3>

    $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:-O0>
    $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:-g3>
    $<$<AND:$<COMPILE_LANGUAGE:ASM>,$<CONFIG:Debug>>:-g>
    ###########################################################################
    #                                 Release                                 #
    ###########################################################################
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release>>:-O2>
    $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Release>>:-O2>
    $<$<AND:$<COMPILE_LANGUAGE:ASM>,$<CONFIG:Release>>:-warn>

    ###########################################################################
    #                                 warnings                                #
    ###########################################################################
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
  )



  #############################################################################
  #                         Flags dependent on language                       #
  #############################################################################
  target_compile_options(${project_name} INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-pedantic-errors>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    )
  #target_compile_options(${project_name} INTERFACE )
  #add_compile_options($<$<COMPILE_LANGUAGE:CXX>:${WARNINGS}>)

endfunction()


###############################################################################
#                           add compiler definitions                          #
###############################################################################

function(set_stm32_compiler_definition project_name)
  set(STM_COMPILER_DEF
    -DDEBUG
    -DUSE_FULL_ASSERT
    -DSTM32F10X_MD
    -DUSE_STDPERIPH_DRIVER
    -DHSE_VALUE=16000000
    )

  target_compile_definitions(${project_name} INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:DEBUG>
    $<$<COMPILE_LANGUAGE:C>:DEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:USE_FULL_ASSERT>
    $<$<COMPILE_LANGUAGE:C>:USE_FULL_ASSERT>
    $<$<COMPILE_LANGUAGE:CXX>:STM32F103xB>
    $<$<COMPILE_LANGUAGE:C>:STM32F103xB>
    # $<$<COMPILE_LANGUAGE:CXX>:STM32F10X_MD>
    # $<$<COMPILE_LANGUAGE:C>:STM32F10X_MD>
    $<$<COMPILE_LANGUAGE:CXX>:HSE_VALUE=16000000>
    $<$<COMPILE_LANGUAGE:C>:HSE_VALUE=16000000>
    )

  #target_compile_definitions(${project_name} INTERFACE ${STM_COMPILER_DEF})

endfunction()


function(set_stm32_linker_flags project_name)

  # set(STM_LINK_FLAGS
  #   -L${CMAKE_SOURCE_DIR}/config/mem
  #   -TSTM32F103XB_FLASH.ld
  #   #-Tsections.ld
  #   #-Tlibs.ld
  #   #-nostartfiles
  #   -Xlinker
  #   --gc-sections
  #   -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${project_name}.map
  #   --specs=nano.specs
  #   )
  #
  #target_link_options(${project_name} INTERFACE ${STM_LINK_FLAGS}   )
  target_link_options(${project_name}
    INTERFACE
    -L${CMAKE_SOURCE_DIR}/config/mem
    -TSTM32F103XB_FLASH.ld
    --specs=nano.specs
    -nostartfiles
    -ffreestanding
  #-flto
  #-nostdlib
  )

endfunction()

# function(add_cpp_features project_name)
#   target_compile_features(${project_name} INTERFACE cxx_std_17)
# endfunction(add_cpp_features)
