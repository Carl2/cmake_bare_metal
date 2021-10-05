set(MCU_COMPILER_TYPE "cortex-m4")
set(MCU_FLOAT True) # IF this is False, it will use soft instead
set(MCU_COMPILER_FPU_DEV "fpv5-sp-d16")
set(MCU_COMPILER_INST_SET "-mthumb")

set(MCU_DEF_MCU "STM32F411xE")
set(MCU_DEF_HAL_DRIVER "USE_HAL_DRIVER")


set(FLAVOR STEPPER CACHE STRING "Flavor(PH) to use")
set_property(CACHE FLAVOR PROPERTY STRINGS STEPPER BLACK_PILL)
