set(PICO_INIT_CMAKE "$ENV{PICO_SDK_PATH}/pico_sdk_init.cmake")

if(NOT EXISTS ${PICO_INIT_CMAKE})
  message(FATAL_ERROR "cannot find ${PICO_INIT_CMAKE}")
endif()

include(${PICO_INIT_CMAKE})
