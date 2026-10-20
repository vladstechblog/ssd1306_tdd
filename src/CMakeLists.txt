add_library(displayLib STATIC SSD1306.c)
target_include_directories(displayLib PUBLIC include)
target_link_libraries(displayLib PUBLIC pico_stdlib)
