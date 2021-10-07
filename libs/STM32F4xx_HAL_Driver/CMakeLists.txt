set(LIB_NAME "Stm32MCUSupport")

# c files template not used
#  Src/stm32f4xx_hal_timebase_rtc_alarm_template.c
#  Src/stm32f4xx_hal_timebase_rtc_wakeup_template.c
#  Src/stm32f4xx_hal_timebase_tim_template.c
#  Src/Legacy/stm32f4xx_hal_can.c
#  Header templates not used
#   ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32_assert_template.h
#  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_conf_template.h

add_library(${LIB_NAME} OBJECT

  Src/stm32f4xx_hal.c
  Src/stm32f4xx_hal_adc.c
  Src/stm32f4xx_hal_adc_ex.c
  Src/stm32f4xx_hal_can.c
  Src/stm32f4xx_hal_cec.c
  Src/stm32f4xx_hal_cortex.c
  Src/stm32f4xx_hal_crc.c
  Src/stm32f4xx_hal_cryp.c
  Src/stm32f4xx_hal_cryp_ex.c
  Src/stm32f4xx_hal_dac.c
  Src/stm32f4xx_hal_dac_ex.c
  Src/stm32f4xx_hal_dcmi.c
  Src/stm32f4xx_hal_dcmi_ex.c
  Src/stm32f4xx_hal_dfsdm.c
  Src/stm32f4xx_hal_dma.c
  Src/stm32f4xx_hal_dma2d.c
  Src/stm32f4xx_hal_dma_ex.c
  Src/stm32f4xx_hal_dsi.c
  Src/stm32f4xx_hal_eth.c
  Src/stm32f4xx_hal_exti.c
  Src/stm32f4xx_hal_flash.c
  Src/stm32f4xx_hal_flash_ex.c
  Src/stm32f4xx_hal_flash_ramfunc.c
  Src/stm32f4xx_hal_fmpi2c.c
  Src/stm32f4xx_hal_fmpi2c_ex.c
  Src/stm32f4xx_hal_fmpsmbus.c
  Src/stm32f4xx_hal_fmpsmbus_ex.c
  Src/stm32f4xx_hal_gpio.c
  Src/stm32f4xx_hal_hash.c
  Src/stm32f4xx_hal_hash_ex.c
  Src/stm32f4xx_hal_hcd.c
  Src/stm32f4xx_hal_i2c.c
  Src/stm32f4xx_hal_i2c_ex.c
  Src/stm32f4xx_hal_i2s.c
  Src/stm32f4xx_hal_i2s_ex.c
  Src/stm32f4xx_hal_irda.c
  Src/stm32f4xx_hal_iwdg.c
  Src/stm32f4xx_hal_lptim.c
  Src/stm32f4xx_hal_ltdc.c
  Src/stm32f4xx_hal_ltdc_ex.c
  Src/stm32f4xx_hal_mmc.c
  #Src/stm32f4xx_hal_msp_template.c
  Src/stm32f4xx_hal_nand.c
  Src/stm32f4xx_hal_nor.c
  Src/stm32f4xx_hal_pccard.c
  Src/stm32f4xx_hal_pcd.c
  Src/stm32f4xx_hal_pcd_ex.c
  Src/stm32f4xx_hal_pwr.c
  Src/stm32f4xx_hal_pwr_ex.c
  Src/stm32f4xx_hal_qspi.c
  Src/stm32f4xx_hal_rcc.c
  Src/stm32f4xx_hal_rcc_ex.c
  Src/stm32f4xx_hal_rng.c
  Src/stm32f4xx_hal_rtc.c
  Src/stm32f4xx_hal_rtc_ex.c
  Src/stm32f4xx_hal_sai.c
  Src/stm32f4xx_hal_sai_ex.c
  Src/stm32f4xx_hal_sd.c
  Src/stm32f4xx_hal_sdram.c
  Src/stm32f4xx_hal_smartcard.c
  Src/stm32f4xx_hal_smbus.c
  Src/stm32f4xx_hal_spdifrx.c
  Src/stm32f4xx_hal_spi.c
  Src/stm32f4xx_hal_sram.c
  Src/stm32f4xx_hal_tim.c
  Src/stm32f4xx_hal_tim_ex.c
  Src/stm32f4xx_hal_uart.c
  Src/stm32f4xx_hal_usart.c
  Src/stm32f4xx_hal_wwdg.c
  Src/stm32f4xx_ll_adc.c
  Src/stm32f4xx_ll_crc.c
  Src/stm32f4xx_ll_dac.c
  Src/stm32f4xx_ll_dma.c
  Src/stm32f4xx_ll_dma2d.c
  Src/stm32f4xx_ll_exti.c
  Src/stm32f4xx_ll_fmc.c
  Src/stm32f4xx_ll_fmpi2c.c
  Src/stm32f4xx_ll_fsmc.c
  Src/stm32f4xx_ll_gpio.c
  Src/stm32f4xx_ll_i2c.c
  Src/stm32f4xx_ll_lptim.c
  Src/stm32f4xx_ll_pwr.c
  Src/stm32f4xx_ll_rcc.c
  Src/stm32f4xx_ll_rng.c
  Src/stm32f4xx_ll_rtc.c
  Src/stm32f4xx_ll_sdmmc.c
  Src/stm32f4xx_ll_spi.c
  Src/stm32f4xx_ll_tim.c
  Src/stm32f4xx_ll_usart.c
  Src/stm32f4xx_ll_usb.c
  Src/stm32f4xx_ll_utils.c

  )


message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(${LIB_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/Legacy/stm32_hal_legacy.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/Legacy/stm32f4xx_hal_can_legacy.h

  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_adc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_adc_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_can.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_cec.h

  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_cortex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_crc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_cryp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_cryp_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_dac.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_dac_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_dcmi.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_dcmi_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_def.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_dfsdm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_dma.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_dma2d.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_dma_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_dsi.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_eth.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_exti.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_flash.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_flash_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_flash_ramfunc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_fmpi2c.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_fmpi2c_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_fmpsmbus.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_fmpsmbus_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_gpio.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_gpio_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_hash.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_hash_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_hcd.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_i2c.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_i2c_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_i2s.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_i2s_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_irda.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_iwdg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_lptim.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_ltdc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_ltdc_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_mmc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_nand.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_nor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_pccard.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_pcd.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_pcd_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_pwr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_pwr_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_qspi.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_rcc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_rcc_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_rng.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_rtc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_rtc_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_sai.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_sai_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_sd.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_sdram.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_smartcard.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_smbus.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_spdifrx.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_spi.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_sram.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_tim.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_tim_ex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_uart.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_usart.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_hal_wwdg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_adc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_bus.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_cortex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_crc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_dac.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_dma.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_dma2d.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_exti.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_fmc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_fmpi2c.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_fsmc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_gpio.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_i2c.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_iwdg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_lptim.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_pwr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_rcc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_rng.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_rtc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_sdmmc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_spi.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_system.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_tim.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_usart.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_usb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_utils.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc/stm32f4xx_ll_wwdg.h
  )


target_include_directories(${LIB_NAME} PUBLIC Inc ${CMAKE_SOURCE_DIR}/${CONF_DIR})

target_link_libraries(${LIB_NAME} PUBLIC
  #project_sanitizer
  CMSIS
  compile_flags
  compile_definitions
  linker_flags
  extra_conf
  #TODO: More to come
  )
