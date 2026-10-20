set(CPPUTEST_REPO "$ENV{HOME}/cpputest")

if(NOT EXISTS ${CPPUTEST_REPO})
  message(FATAL_ERROR "cannot find ${CPPUTEST_REPO}")
endif()

set(FETCHCONTENT_TRY_FIND_PACKAGE NEVER)
include(FetchContent)

FetchContent_Declare(
  CppUTest
  URL ${CPPUTEST_REPO}
)

FetchContent_MakeAvailable(CppUTest)

unset(FETCHCONTENT_TRY_FIND_PACKAGE)
