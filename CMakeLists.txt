cmake_minimum_required(VERSION 3.31)

if(NOT DEFINED ENV{PICO_SDK_PATH})
  message(FATAL_ERROR "PICO_SKD_PATH environment variable is not defined")
elseif(NOT EXISTS $ENV{PICO_SDK_PATH})
  message(FATAL_ERROR "Cannot find $ENV{PICO_SDK_PATH}")
endif()

include(include_pico_init.cmake)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "Insource build detected. Use a different build directory")
endif()

project(ssd1306tdd C CXX ASM)
pico_sdk_init()

add_subdirectory(src)
add_subdirectory(test EXCLUDE_FROM_ALL)
