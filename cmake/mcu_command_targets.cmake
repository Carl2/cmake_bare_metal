###############################################################################
#                             Make a flash target                             #
#   This will make a flash target fot the executable target
###############################################################################
function(make_mcu_cmd_target)



  cmake_parse_arguments(TARGET "" "NAME" "" ${ARGN})

  add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy  -O ihex ${TARGET_NAME} ${TARGET_NAME}.hex
    VERBATIM
  )

  get_target_property(BIN_DIR ${TARGET_NAME} BINARY_DIR)

  add_custom_target(flash
    COMMAND bash -c "st-flash --format=ihex write ${BIN_DIR}/${TARGET_NAME}.hex"
    VERBATIM
    )

endfunction()


###############################################################################
#                                Make ocd_flash
#                                Flash using picoprobe or somrthing else
###############################################################################
function(make_openocd_target)
  cmake_parse_arguments(TARGET "" "NAME;OPENOCD_PATH;OPENOCD_INTERFACE;OPENOCD_TARGET" "" ${ARGN})

  get_target_property(BIN_DIR ${TARGET_NAME} BINARY_DIR)

  add_custom_target(ocd_flash
    COMMAND bash -c "${CMAKE_SOURCE_DIR}/tools/openocd/run_openocd.sh -o ${TARGET_OPENOCD_PATH} \
-i ${TARGET_OPENOCD_INTERFACE} \
-t ${TARGET_OPENOCD_TARGET} \
-e ${BIN_DIR}/${TARGET_NAME}"
    VERBATIM
    )
endfunction(make_openocd_target)
