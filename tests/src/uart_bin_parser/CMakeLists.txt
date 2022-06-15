set(EXEC_NAME "uart_bin_parser")
find_package(GTest)
find_package(fmt)
find_package(sml)

include_directories(
  ${PB_SOURCE_DIR}
  ${PB_SOURCE_DIR}/uart_bin_parser/cpp/
  )

set(SRC
  main_test.cpp
  crc_check.cpp
  )


add_executable(${EXEC_NAME} ${SRC})

target_link_libraries(${EXEC_NAME}
  GTest::gtest
  fmt::fmt
  sml::sml
  )

add_test(
  NAME ${EXEC_NAME}
  COMMAND ${EXEC_NAME}
  )
