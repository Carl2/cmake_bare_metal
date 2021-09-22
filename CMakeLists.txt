cmake_minimum_required(VERSION 3.15)

project ( PH )

set(FLAVOR ALFA CACHE STRING "Flavor(PH) to use")
set_property(CACHE FLAVOR PROPERTY STRINGS ALFA BETA GAMMA)

set(CONFIG CONF_ONE CACHE STRING "Config  to use")
set_property(CACHE CONFIG PROPERTY STRINGS CONF_ONE CONF_TWO)

# Link Time Optimization (LTO)
include("cmake/StandardProjectSettings.cmake")

# Compiler warnings
include("cmake/CompilerWarnings.cmake")
include("cmake/Sanitizers.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra ")



add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

set(ENABLE_CLANG_TIDY ON)
include("cmake/StaticAnalyzers.cmake")

add_library(project_warnings INTERFACE)
set_project_warnings( project_warnings )

add_library(project_sanitizer INTERFACE)
enable_sanitizers(project_sanitizer)




include_directories(
include
)

add_subdirectory(libs)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
