cmake_minimum_required(VERSION 3.15)
include(cmake/arm-none-eabi-toolchain.cmake)
include(CMakeDependentOption)
project ( PH  CXX C ASM )

include(cmake/arm_stm32.cmake)
include(cmake/StandardProjectSettings.cmake)
#TODO: Now im just going to use the M4...



include("cmake/MCU_selection.cmake")
set_project_config( MCU m4 CONF ${CONF_DIR})
#TODO: The files (MCU_CONFIG) are using set without PARENT SCOPE
# which means it needs to be included here , otherwise
# the compiler flags what see it the settings.
include(${CMAKE_SOURCE_DIR}/cmake/mcu_conf/${MCU_CONFIG})


###############################################################################
#                           See mcu_command_target.cmake
#                           This will be used to create make ocd_flash
###############################################################################
set(OPENOCD_PATH "/usr/bin" CACHE STRING "Where to find openocd")
set(OPENOCD_INTERFACE "cmsis-dap" CACHE STRING "Interface to use ")


add_library(compile_flags INTERFACE)
set_stm32_compiler_options(compile_flags)
add_dependencies(compile_flags CBM_tests)

add_library(compile_definitions INTERFACE)
set_stm32_compiler_definition(compile_definitions)
add_library(linker_flags INTERFACE)
set_stm32_linker_flags(linker_flags)



#TODO: This is under dev.
set(CONFIG CONF_ONE CACHE STRING "Config  to use")
set_property(CACHE CONFIG PROPERTY STRINGS CONF_ONE CONF_TWO)





# Link Time Optimization (LTO)
#include("cmake/StandardProjectSettings.cmake")
# Compiler warnings
#include("cmake/CompilerWarnings.cmake")
#include("cmake/Sanitizers.cmake")




add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

set(ENABLE_CLANG_TIDY OFF)
include("cmake/StaticAnalyzers.cmake")

#add_library(project_warnings INTERFACE)
#set_project_warnings( project_warnings )

#add_library(project_sanitizer INTERFACE)
#enable_sanitizers(project_sanitizer)




# include_directories(
#   include
# )

add_subdirectory(libs)
add_subdirectory(src)


include(ExternalProject)
ExternalProject_Add(CBM_tests
  UPDATE_DISCONNECTED true
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/tests
  BINARY_DIR ${CMAKE_BINARY_DIR}/test_bin
  BUILD_ALWAYS true
  STEP_TARGETS build
  CMAKE_ARGS
     -DPB_SOURCE_DIR=${CMAKE_SOURCE_DIR}/src
     -DPB_WIP_DIR=${CMAKE_SOURCE_DIR}
  INSTALL_COMMAND ""
  )
