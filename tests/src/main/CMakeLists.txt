set(EXEC_NAME "hw_tests")
find_package(GTest)
find_package(fmt)
find_package(sml)

include_directories(
  ${PB_SOURCE_DIR}
  )

set(SRC
  main_test.cpp
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
