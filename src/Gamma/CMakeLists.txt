cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")




include_directories(
  include
)

###########
# Sources that compiles
###########
set( SRC main.cpp)


###########
# Add exectutable
###########
add_executable( PH ${SRC} )

###########
# Library linking
###########
target_link_libraries( PH
PRIVATE
project_options
project_warnings
project_sanitizer
)