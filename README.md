# SSD1306 TDD

**WORK IN PROGRESS... Incomplete...**

Designed for 128x64 models.


This is another example project for practicing Test Driver Development with
Raspberry Pi Pico C/C++ SDK.

The purpose of this project to make something a bit more real than just
blinking an LED :P

## Building and Running Tests

Building and running tests on the host target (developer machine).

```shell
git clone https://github.com/vladstechblog/ssd1306_tdd
cd ssd1306_tdd
cmake -B build_host -DPICO_PLATFORM=host
cmake --build build_host/ -t displayTestRunner
./build_host/test/displayTestRunner
```
