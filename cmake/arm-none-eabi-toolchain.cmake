set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_SYSTEM_NAME Generic)


set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
# Use gcc as a precompiler to gcc, that way it reconizes the
# c flags (-O2)
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER_ID_RUN TRUE)
set(CMAKE_CXX_COMPILER_ID_RUN TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
