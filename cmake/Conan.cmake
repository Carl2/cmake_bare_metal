###############################################################################
#                             Conan download help                             #
#  At the moment download for all verions of conan is done.
#  Though this is not really necessary for conan2, but conan_version function
#  is included in the conan.cmake, this could be made a function inside this file,
#  in that case only download is only needed for conan < 2.
###############################################################################
macro(download_conan_help)

  message(STATUS "########## ${CMAKE_SOURCE_DIR}")
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(
      STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(
      DOWNLOAD
      #"https://raw.githubusercontent.com/conan-io/cmake-conan/release/0.18/conan.cmake"
      "https://raw.githubusercontent.com/conan-io/cmake-conan/develop/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake"
      EXPECTED_HASH
      #SHA256=5cdb3042632da3efff558924eecefd580a0e786863a857ca097c3d1d43df5dcd
      SHA256=c284fcb53d9106351581efdd914b8f4869d351f603312e2d75575a5b0b536deb
      TLS_VERIFY ON)
  endif()

  set(ENV{CONAN_REVISIONS_ENABLED} 1)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

  message(STATUS "Include ${CMAKE_BINARY_DIR}/conan.cmake")
  include(${CMAKE_BINARY_DIR}/conan.cmake)
endmacro()

###############################################################################
#                                Conan2 install                               #
#   Will generate the conan2 install files.
#   This will use build type: Debug and CXX_Standard: (same as Build) 20
###############################################################################

macro(conan2_install)
  cmake_parse_arguments(CONAN "" "INSTALL_DIR;PROFILE" "" ${ARGN} )
  message(STATUS "Conan Compiler standard: compiler.cppstd=gnu${CMAKE_CXX_STANDARD}")
  set(CONAN_GENERATORS_FOLDER "${CONAN_INSTALL_DIR}/out")

  execute_process(
    COMMAND conan install ${CONAN_INSTALL_DIR} --output-folder=${CONAN_INSTALL_DIR}/out --build=missing -s build_type=Debug -s compiler.cppstd=gnu${CMAKE_CXX_STANDARD}
    WORKING_DIRECTORY ${CONAN_INSTALL_DIR}
    RESULT_VARIABLE return_code
    ECHO_ERROR_VARIABLE
  )

endmacro(conan2_install)




###############################################################################
#             Will evaluate what version of conan
# is currently running, depending if we use the > 2
# execute a process to generate the cmake files, and include the
# toolchain file.
#
# For conan < 2 , the conan.cmake (which is downloaded earlier) , and call
# to run_conan(),
###############################################################################
macro(generate_conan_files)
  cmake_parse_arguments(CONAN "" "VERSION" "" ${ARGN} )
  message(STATUS "Got ${CONAN_VERSION}")
  ##set(CONAN_FILE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/conan_files")
  #The problem here is that if the version is > 2
  # then we need to use the new way of doing things.
  if("2" VERSION_LESS ${CONAN_VERSION})
    message(STATUS "Conan Version > 2 ${CMAKE_CURRENT_SOURCE_DIR}")
    conan2_install(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/conan_files/conan2)
    include("${CONAN_GENERATORS_FOLDER}/conan_toolchain.cmake")
  else()
    message(STATUS "Conan Version < 2")
    conan_cmake_autodetect(settings)
    conan_cmake_install(
      PATH_OR_REFERENCE ${CMAKE_CURRENT_SOURCE_DIR}/conan_files/conan1
      BUILD missing
      SETTINGS ${settings}
    )
  endif()
endmacro(generate_conan_files)
