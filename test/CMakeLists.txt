include(include_local_cpputest_git_repo.cmake)

add_executable(displayTestRunner displayTestRunner.cpp ../src/SSD1306.c)
target_include_directories(displayTestRunner PUBLIC include ../src/include)
target_link_libraries(
  displayTestRunner
  PUBLIC
  CppUTest
  CppUTestExt
)
