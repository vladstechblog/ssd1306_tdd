include(include_local_cpputest_git_repo.cmake)

add_executable(displayTestRunner displayTestRunner.cpp)
target_include_directories(displayTestRunner PUBLIC)
target_link_libraries(
  displayTestRunner
  PUBLIC
  displayLib
  CppUTest
  CppUTestExt
)
