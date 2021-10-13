cmake_minimum_required(VERSION 3.15)
include(cmake/arm-none-eabi-toolchain.cmake)
include(CMakeDependentOption)
project ( PH  CXX C ASM )

include(cmake/arm_stm32.cmake)
#include(cmake/standard_prj_settings.cmake)
#TODO: Now im just going to use the M4...


include("cmake/MCU_selection.cmake")
set_project_config( MCU m4 CONF ${CONF_DIR})
#TODO: The files (MCU_CONFIG) are using set without PARENT SCOPE
# which means it needs to be included here , otherwise
# the compiler flags what see it the settings.
include(${CMAKE_SOURCE_DIR}/cmake/mcu_conf/${MCU_CONFIG})


add_library(compile_flags INTERFACE)
set_stm32_compiler_options(compile_flags)
add_library(compile_definitions INTERFACE)
set_stm32_compiler_definition(compile_definitions)
add_library(linker_flags INTERFACE)
set_stm32_linker_flags(linker_flags)




# set(FLAVOR STEPPER CACHE STRING "Flavor(PH) to use")
# set_property(CACHE FLAVOR PROPERTY STRINGS STEPPER BLACK_PILL ALFA BETA GAMMA)

set(CONFIG CONF_ONE CACHE STRING "Config  to use")
set_property(CACHE CONFIG PROPERTY STRINGS CONF_ONE CONF_TWO)

set(CONF_DIR "src/black_pill/Inc" CACHE STRING "Configure directory")
set_property(CACHE CONF_DIR PROPERTY STRINGS "/src/black_pill/Inc" "/src/Alfa/Inc" "/src/ph_disco/Inc")




# Link Time Optimization (LTO)
#include("cmake/StandardProjectSettings.cmake")
# Compiler warnings
#include("cmake/CompilerWarnings.cmake")
#include("cmake/Sanitizers.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra ")



add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

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

#enable_testing()
#add_subdirectory(tests)
