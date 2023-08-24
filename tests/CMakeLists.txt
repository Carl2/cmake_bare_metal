cmake_minimum_required(VERSION 3.15)

project ( Pico_bike_tests CXX C ASM)

#set(PH_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../src")
#set(PH_WIP_DIR "${CMAKE_SOURCE_DIR}/..")


set(CMAKE_CXX_STANDARD  20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic-errors")

set(CMAKE_BUILD_TYPE "Debug")

message(STATUS "PB_WIP_DIR ${PB_WIP_DIR} ${PB_SOURCE_DIR}")


option(ENABLE_CONAN "Use Conan for dependency management" ON)
if(ENABLE_CONAN)

  include(${PB_WIP_DIR}/cmake/Conan.cmake)
  download_conan_help()

  conan_version(CONAN_VERSION)
  generate_conan_files(VERSION ${CONAN_VERSION})



endif()

# option(ENABLE_CONAN "Use Conan for dependency management" ON)
# if(ENABLE_CONAN)
#   include(${PB_WIP_DIR}/cmake/Conan.cmake)
#   run_conan()
# endif()
# message(STATUS "${CMAKE_BINARY_DIR}" )


find_package(GTest)
find_package(fmt)
find_package(sml)

#find_package( Boost  COMPONENTS system program_options REQUIRED )

include_directories(
  include
  #${Boost_INCLUDE_DIR}
  )




enable_testing()
add_subdirectory(src)
