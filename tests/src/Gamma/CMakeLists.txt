cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra ")
find_package(GTest REQUIRED)
find_package(Threads)

set(TARGET_NAME "Gamma")

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src  )

###########
# Sources that compiles
###########
set( SRC test_${TARGET_NAME}.cpp)


###########
# Add exectutable
###########
add_executable( ${PROJECT_NAME}_${TARGET_NAME}_test ${SRC} )

###########
# Library linking
###########
target_link_libraries( ${PROJECT_NAME}_${TARGET_NAME}_test
    gtest
    ${CMAKE_THREAD_LIBS_INIT}
  )



add_test(
  NAME ${TARGET_NAME}_test
  COMMAND ${PROJECT_NAME}_${TARGET_NAME}_test
)
