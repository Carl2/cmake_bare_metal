set(MCU_COMPILER_TYPE "cortex-m4")
set(MCU_FLOAT True) # IF this is False, it will use soft instead
set(MCU_COMPILER_FPU_DEV "fpv5-sp-d16")
set(MCU_COMPILER_INST_SET "-mthumb")

set(MCU_DEF_MCU "STM32F401xC")
set(MCU_DEF_HAL_DRIVER "USE_HAL_DRIVER")


set(FLAVOR SML_TEST CACHE STRING "Flavor(PH) to use")
set_property(CACHE FLAVOR PROPERTY STRINGS STEPPER SML_TEST)

#Linker options
set(MCU_LINKER_MAP_FILE "${CMAKE_CURRENT_BINARY_DIR}/${FLAVOR}.map")
set(MCU_MEMORY_LAYOUT_DIR "${CMAKE_SOURCE_DIR}/libs/conf/mem/")
set(MCU_MEMORY_LAYOUT_FILES "STM32F401CCUX_FLASH.ld")


# - nosys.specs just defines that system calls should be implemented as stubs
# - nano.specs defines the system include path and library parameters to use newlib-nano
# - rdimon.specs, -lrdimon is passed as the libgloss part of the standard library.
#    This basically means that you can use system calls (and also printf),
#    but this relies on a debugger being attached, and the CPU may crash if no debugger is present.
set(MCU_LINKER_SPEC "nosys.specs") # nano by default nosys.spec or rdimon.spec...
