###############################################################################
#                         MCU device support external                         #
###############################################################################
# function(f4_device_support)

#   # set(options )
#   # set(oneValueArgs DESTINATION)
#   # set(multiValueArgs )
#   #{CMAKE_SOURCE_DIR}/external/cmsis_device_f4
#   include(FetchContent)
#   cmake_parse_arguments(DEVICE "" "DESTINATION" "" ${ARGN} )
#   set(MCU_SUPPORT_DIR "${DEVICE_DESTINATION}/cmsis_device_f4")




#   FetchContent_Declare(stm32F4_mcu_support
#     GIT_REPOSITORY    git@github.com:STMicroelectronics/cmsis_device_f4.git
#     GIT_TAG           v2.6.7
#     GIT_SHALLOW       True
#     SOURCE_DIR        ${MCU_SUPPORT_DIR}
#     # CONFIGURE_COMMAND ""
#     # BUILD_COMMAND ""
#     # INSTALL_COMMAND ""
#     )
#   #TODO: The Startup file does not need to be saved, the conf can be found
#   # in here.
#   set(STM_DEVICE_SUPPORT_INCLUDE "${DEVICE_DESTINATION}/Include"  PARENT_SCOPE )
#   FetchContent_MakeAvailable(stm32F4_mcu_support)
# endfunction(f4_device_support)


###############################################################################
#                              CMSIS HAL support                              #
# Since the HAL library don't have a build we neeed to use our own cmake
# file
###############################################################################
function(f4_HAL_support)
  #f4_device_support(DESTINATION "${CMAKE_SOURCE_DIR}/external/cmsis_device_f4" )

  include(FetchContent)
  cmake_parse_arguments(DEVICE "" "DESTINATION;CONFIG_DIR" "" ${ARGN} )



  set(LIB_NAME "stm32F4_HAL_support_lib")
  set(HAL_DIR "${DEVICE_DESTINATION}/STM32F4xx_HAL_DRIVER")
  set(HAL_DIR_INC "${HAL_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc")
  set(HAL_DIR_SRC "${HAL_DIR}/Drivers/STM32F4xx_HAL_Driver/Src")
  set(HAL_DIR_CORE "${HAL_DIR}/Drivers/CMSIS/Core/Include/")

  # The device headers
  set(HAL_DEVICE_INC "${HAL_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include/")
  set(HAL_CORE_INC "${HAL_DIR}/Drivers/CMSIS/Core/Include")

  FetchContent_Declare(stm32CubeF4_HAL
    GIT_REPOSITORY    git@github.com:STMicroelectronics/STM32CubeF4.git
    GIT_TAG           v1.26.2
    GIT_SHALLOW       True
    SOURCE_DIR        ${HAL_DIR}
    )

  FetchContent_MakeAvailable(stm32CubeF4_HAL)

  #############################################################################
  #                           Creating a lib for f1                           #
  #############################################################################
  add_library(${LIB_NAME} STATIC

  ${HAL_DIR_SRC}/stm32f4xx_hal.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_adc.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_adc_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_can.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_cec.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_cortex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_crc.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_cryp.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_cryp_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_dac.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_dac_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_dcmi.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_dcmi_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_dfsdm.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_dma.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_dma2d.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_dma_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_dsi.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_eth.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_exti.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_flash.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_flash_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_flash_ramfunc.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_fmpi2c.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_fmpi2c_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_fmpsmbus.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_fmpsmbus_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_gpio.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_hash.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_hash_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_hcd.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_i2c.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_i2c_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_i2s.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_i2s_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_irda.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_iwdg.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_lptim.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_ltdc.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_ltdc_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_mmc.c
  #${HAL_DIR_SRC}/stm32f4xx_hal_msp_template.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_nand.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_nor.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_pccard.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_pcd.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_pcd_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_pwr.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_pwr_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_qspi.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_rcc.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_rcc_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_rng.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_rtc.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_rtc_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_sai.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_sai_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_sd.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_sdram.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_smartcard.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_smbus.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_spdifrx.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_spi.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_sram.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_tim.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_tim_ex.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_uart.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_usart.c
  ${HAL_DIR_SRC}/stm32f4xx_hal_wwdg.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_adc.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_crc.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_dac.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_dma.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_dma2d.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_exti.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_fmc.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_fmpi2c.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_fsmc.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_gpio.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_i2c.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_lptim.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_pwr.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_rcc.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_rng.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_rtc.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_sdmmc.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_spi.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_tim.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_usart.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_usb.c
  ${HAL_DIR_SRC}/stm32f4xx_ll_utils.c

  )


 message(STATUS "Creating target stm32F4_mcu_support_lib")

target_sources(${LIB_NAME} PUBLIC
   ${HAL_DIR_INC}/Legacy/stm32_hal_legacy.h
   ${HAL_DIR_INC}/Legacy/stm32f4xx_hal_can_legacy.h

   ${HAL_DIR_INC}/stm32f4xx_hal.h
   ${HAL_DIR_INC}/stm32f4xx_hal_adc.h
   ${HAL_DIR_INC}/stm32f4xx_hal_adc_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_can.h
   ${HAL_DIR_INC}/stm32f4xx_hal_cec.h

   ${HAL_DIR_INC}/stm32f4xx_hal_cortex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_crc.h
   ${HAL_DIR_INC}/stm32f4xx_hal_cryp.h
   ${HAL_DIR_INC}/stm32f4xx_hal_cryp_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_dac.h
   ${HAL_DIR_INC}/stm32f4xx_hal_dac_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_dcmi.h
   ${HAL_DIR_INC}/stm32f4xx_hal_dcmi_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_def.h
   ${HAL_DIR_INC}/stm32f4xx_hal_dfsdm.h
   ${HAL_DIR_INC}/stm32f4xx_hal_dma.h
   ${HAL_DIR_INC}/stm32f4xx_hal_dma2d.h
   ${HAL_DIR_INC}/stm32f4xx_hal_dma_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_dsi.h
   ${HAL_DIR_INC}/stm32f4xx_hal_eth.h
   ${HAL_DIR_INC}/stm32f4xx_hal_exti.h
   ${HAL_DIR_INC}/stm32f4xx_hal_flash.h
   ${HAL_DIR_INC}/stm32f4xx_hal_flash_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_flash_ramfunc.h
   ${HAL_DIR_INC}/stm32f4xx_hal_fmpi2c.h
   ${HAL_DIR_INC}/stm32f4xx_hal_fmpi2c_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_fmpsmbus.h
   ${HAL_DIR_INC}/stm32f4xx_hal_fmpsmbus_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_gpio.h
   ${HAL_DIR_INC}/stm32f4xx_hal_gpio_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_hash.h
   ${HAL_DIR_INC}/stm32f4xx_hal_hash_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_hcd.h
   ${HAL_DIR_INC}/stm32f4xx_hal_i2c.h
   ${HAL_DIR_INC}/stm32f4xx_hal_i2c_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_i2s.h
   ${HAL_DIR_INC}/stm32f4xx_hal_i2s_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_irda.h
   ${HAL_DIR_INC}/stm32f4xx_hal_iwdg.h
   ${HAL_DIR_INC}/stm32f4xx_hal_lptim.h
   ${HAL_DIR_INC}/stm32f4xx_hal_ltdc.h
   ${HAL_DIR_INC}/stm32f4xx_hal_ltdc_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_mmc.h
   ${HAL_DIR_INC}/stm32f4xx_hal_nand.h
   ${HAL_DIR_INC}/stm32f4xx_hal_nor.h
   ${HAL_DIR_INC}/stm32f4xx_hal_pccard.h
   ${HAL_DIR_INC}/stm32f4xx_hal_pcd.h
   ${HAL_DIR_INC}/stm32f4xx_hal_pcd_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_pwr.h
   ${HAL_DIR_INC}/stm32f4xx_hal_pwr_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_qspi.h
   ${HAL_DIR_INC}/stm32f4xx_hal_rcc.h
   ${HAL_DIR_INC}/stm32f4xx_hal_rcc_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_rng.h
   ${HAL_DIR_INC}/stm32f4xx_hal_rtc.h
   ${HAL_DIR_INC}/stm32f4xx_hal_rtc_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_sai.h
   ${HAL_DIR_INC}/stm32f4xx_hal_sai_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_sd.h
   ${HAL_DIR_INC}/stm32f4xx_hal_sdram.h
   ${HAL_DIR_INC}/stm32f4xx_hal_smartcard.h
   ${HAL_DIR_INC}/stm32f4xx_hal_smbus.h
   ${HAL_DIR_INC}/stm32f4xx_hal_spdifrx.h
   ${HAL_DIR_INC}/stm32f4xx_hal_spi.h
   ${HAL_DIR_INC}/stm32f4xx_hal_sram.h
   ${HAL_DIR_INC}/stm32f4xx_hal_tim.h
   ${HAL_DIR_INC}/stm32f4xx_hal_tim_ex.h
   ${HAL_DIR_INC}/stm32f4xx_hal_uart.h
   ${HAL_DIR_INC}/stm32f4xx_hal_usart.h
   ${HAL_DIR_INC}/stm32f4xx_hal_wwdg.h
   ${HAL_DIR_INC}/stm32f4xx_ll_adc.h
   ${HAL_DIR_INC}/stm32f4xx_ll_bus.h
   ${HAL_DIR_INC}/stm32f4xx_ll_cortex.h
   ${HAL_DIR_INC}/stm32f4xx_ll_crc.h
   ${HAL_DIR_INC}/stm32f4xx_ll_dac.h
   ${HAL_DIR_INC}/stm32f4xx_ll_dma.h
   ${HAL_DIR_INC}/stm32f4xx_ll_dma2d.h
   ${HAL_DIR_INC}/stm32f4xx_ll_exti.h
   ${HAL_DIR_INC}/stm32f4xx_ll_fmc.h
   ${HAL_DIR_INC}/stm32f4xx_ll_fmpi2c.h
   ${HAL_DIR_INC}/stm32f4xx_ll_fsmc.h
   ${HAL_DIR_INC}/stm32f4xx_ll_gpio.h
   ${HAL_DIR_INC}/stm32f4xx_ll_i2c.h
   ${HAL_DIR_INC}/stm32f4xx_ll_iwdg.h
   ${HAL_DIR_INC}/stm32f4xx_ll_lptim.h
   ${HAL_DIR_INC}/stm32f4xx_ll_pwr.h
   ${HAL_DIR_INC}/stm32f4xx_ll_rcc.h
   ${HAL_DIR_INC}/stm32f4xx_ll_rng.h
   ${HAL_DIR_INC}/stm32f4xx_ll_rtc.h
   ${HAL_DIR_INC}/stm32f4xx_ll_sdmmc.h
   ${HAL_DIR_INC}/stm32f4xx_ll_spi.h
   ${HAL_DIR_INC}/stm32f4xx_ll_system.h
   ${HAL_DIR_INC}/stm32f4xx_ll_tim.h
   ${HAL_DIR_INC}/stm32f4xx_ll_usart.h
   ${HAL_DIR_INC}/stm32f4xx_ll_usb.h
   ${HAL_DIR_INC}/stm32f4xx_ll_utils.h
   ${HAL_DIR_INC}/stm32f4xx_ll_wwdg.h
  )


target_include_directories(${LIB_NAME} PUBLIC
  ${HAL_DIR_INC}
  ${HAL_DEVICE_INC}
  ${HAL_CORE_INC}
  ${DEVICE_CONFIG_DIR}
  #${STM_DEVICE_SUPPORT_INCLUDE}

  )

target_link_libraries(${LIB_NAME} PUBLIC
  #project_sanitizer
  compile_flags
  compile_definitions
  linker_flags
)

#   #TODO: More to come
#   )

# add_dependencies(${LIB_NAME} stm32F4_mcu_support ${stm32F4_HAL_support});


endfunction(f4_HAL_support)




###############################################################################
#                                Support for stm32Cubef1                      #
###############################################################################

function(f1_HAL_support)


  include(FetchContent)
  cmake_parse_arguments(DEVICE "" "DESTINATION;CONFIG_DIR" "" ${ARGN} )



  set(LIB_NAME "stm32F1_HAL_support_lib")
  set(HAL_DIR "${DEVICE_DESTINATION}/STM32F1xx_HAL_DRIVER")
  set(HAL_DIR_INC "${HAL_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc")
  set(HAL_DIR_SRC "${HAL_DIR}/Drivers/STM32F1xx_HAL_Driver/Src")
  set(HAL_DIR_CORE "${HAL_DIR}/Drivers/CMSIS/Core/Include/")

  # The device headers
  set(HAL_DEVICE_INC "${HAL_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include/")
  set(HAL_CORE_INC "${HAL_DIR}/Drivers/CMSIS/Core/Include")



  #git@github.com:STMicroelectronics/STM32CubeF4.git
  FetchContent_Declare(stm32CubeF1_HAL
    GIT_REPOSITORY    git@github.com:STMicroelectronics/STM32CubeF1.git
    GIT_TAG           v1.8.4
    GIT_SHALLOW       True
    SOURCE_DIR        ${HAL_DIR}
    )

  FetchContent_MakeAvailable(stm32CubeF1_HAL)


  message(STATUS "HAL_DIR_SRC ${HAL_DIR_SRC}")
    #############################################################################
  #                           Creating a lib for f4                           #
  #############################################################################
  add_library(${LIB_NAME} OBJECT
    ${HAL_DIR_SRC}/Legacy/stm32f1xx_hal_can.c
    ${HAL_DIR_SRC}/stm32f1xx_hal.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_adc.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_adc_ex.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_can.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_cec.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_cortex.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_crc.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_dac.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_dac_ex.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_dma.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_eth.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_exti.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_flash.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_flash_ex.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_gpio.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_gpio_ex.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_hcd.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_i2c.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_i2s.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_irda.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_iwdg.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_mmc.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_nand.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_nor.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_pccard.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_pcd.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_pcd_ex.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_pwr.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_rcc.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_rcc_ex.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_rtc.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_rtc_ex.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_sd.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_smartcard.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_spi.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_sram.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_tim.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_tim_ex.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_uart.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_usart.c
    ${HAL_DIR_SRC}/stm32f1xx_hal_wwdg.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_adc.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_crc.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_dac.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_dma.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_exti.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_fsmc.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_gpio.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_i2c.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_pwr.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_rcc.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_rtc.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_sdmmc.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_spi.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_tim.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_usart.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_usb.c
    ${HAL_DIR_SRC}/stm32f1xx_ll_utils.c
  )


 message(STATUS "Creating target stm32F1_mcu_support_lib")

 # To get source and headers
 # find . -name "*.h" | sed 's|.\/||g' | awk '{print "${HAL_DIR_INC}/"$0}' | grep -v template  | clip
 target_sources(${LIB_NAME} PUBLIC

   ${HAL_DIR_INC}/Legacy/stm32_hal_legacy.h
   ${HAL_DIR_INC}/Legacy/stm32f1xx_hal_can_ex_legacy.h
   ${HAL_DIR_INC}/Legacy/stm32f1xx_hal_can_legacy.h
   ${HAL_DIR_INC}/stm32f1xx_hal.h
   ${HAL_DIR_INC}/stm32f1xx_hal_adc.h
   ${HAL_DIR_INC}/stm32f1xx_hal_adc_ex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_can.h
   ${HAL_DIR_INC}/stm32f1xx_hal_cec.h
   ${HAL_DIR_INC}/stm32f1xx_hal_cortex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_crc.h
   ${HAL_DIR_INC}/stm32f1xx_hal_dac.h
   ${HAL_DIR_INC}/stm32f1xx_hal_dac_ex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_def.h
   ${HAL_DIR_INC}/stm32f1xx_hal_dma.h
   ${HAL_DIR_INC}/stm32f1xx_hal_dma_ex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_eth.h
   ${HAL_DIR_INC}/stm32f1xx_hal_exti.h
   ${HAL_DIR_INC}/stm32f1xx_hal_flash.h
   ${HAL_DIR_INC}/stm32f1xx_hal_flash_ex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_gpio.h
   ${HAL_DIR_INC}/stm32f1xx_hal_gpio_ex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_hcd.h
   ${HAL_DIR_INC}/stm32f1xx_hal_i2c.h
   ${HAL_DIR_INC}/stm32f1xx_hal_i2s.h
   ${HAL_DIR_INC}/stm32f1xx_hal_irda.h
   ${HAL_DIR_INC}/stm32f1xx_hal_iwdg.h
   ${HAL_DIR_INC}/stm32f1xx_hal_mmc.h
   ${HAL_DIR_INC}/stm32f1xx_hal_nand.h
   ${HAL_DIR_INC}/stm32f1xx_hal_nor.h
   ${HAL_DIR_INC}/stm32f1xx_hal_pccard.h
   ${HAL_DIR_INC}/stm32f1xx_hal_pcd.h
   ${HAL_DIR_INC}/stm32f1xx_hal_pcd_ex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_pwr.h
   ${HAL_DIR_INC}/stm32f1xx_hal_rcc.h
   ${HAL_DIR_INC}/stm32f1xx_hal_rcc_ex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_rtc.h
   ${HAL_DIR_INC}/stm32f1xx_hal_rtc_ex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_sd.h
   ${HAL_DIR_INC}/stm32f1xx_hal_smartcard.h
   ${HAL_DIR_INC}/stm32f1xx_hal_spi.h
   ${HAL_DIR_INC}/stm32f1xx_hal_sram.h
   ${HAL_DIR_INC}/stm32f1xx_hal_tim.h
   ${HAL_DIR_INC}/stm32f1xx_hal_tim_ex.h
   ${HAL_DIR_INC}/stm32f1xx_hal_uart.h
   ${HAL_DIR_INC}/stm32f1xx_hal_usart.h
   ${HAL_DIR_INC}/stm32f1xx_hal_wwdg.h
   ${HAL_DIR_INC}/stm32f1xx_ll_adc.h
   ${HAL_DIR_INC}/stm32f1xx_ll_bus.h
   ${HAL_DIR_INC}/stm32f1xx_ll_cortex.h
   ${HAL_DIR_INC}/stm32f1xx_ll_crc.h
   ${HAL_DIR_INC}/stm32f1xx_ll_dac.h
   ${HAL_DIR_INC}/stm32f1xx_ll_dma.h
   ${HAL_DIR_INC}/stm32f1xx_ll_exti.h
   ${HAL_DIR_INC}/stm32f1xx_ll_fsmc.h
   ${HAL_DIR_INC}/stm32f1xx_ll_gpio.h
   ${HAL_DIR_INC}/stm32f1xx_ll_i2c.h
   ${HAL_DIR_INC}/stm32f1xx_ll_iwdg.h
   ${HAL_DIR_INC}/stm32f1xx_ll_pwr.h
   ${HAL_DIR_INC}/stm32f1xx_ll_rcc.h
   ${HAL_DIR_INC}/stm32f1xx_ll_rtc.h
   ${HAL_DIR_INC}/stm32f1xx_ll_sdmmc.h
   ${HAL_DIR_INC}/stm32f1xx_ll_spi.h
   ${HAL_DIR_INC}/stm32f1xx_ll_system.h
   ${HAL_DIR_INC}/stm32f1xx_ll_tim.h
   ${HAL_DIR_INC}/stm32f1xx_ll_usart.h
   ${HAL_DIR_INC}/stm32f1xx_ll_usb.h
   ${HAL_DIR_INC}/stm32f1xx_ll_utils.h
   ${HAL_DIR_INC}/stm32f1xx_ll_wwdg.h


  )


target_include_directories(${LIB_NAME} PUBLIC
  ${HAL_DIR_INC}
  ${HAL_DEVICE_INC}
  ${HAL_CORE_INC}
  ${DEVICE_CONFIG_DIR}
  #${STM_DEVICE_SUPPORT_INCLUDE}

  )

target_link_libraries(${LIB_NAME} PUBLIC
  #project_sanitizer
  compile_flags
  compile_definitions
  linker_flags
)


endfunction(f1_HAL_support)
