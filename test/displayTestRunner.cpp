#include <CppUTest/CommandLineTestRunner.h>
#include "SSD1306CommandsTest.cpp"
#include "SSD1306InitializationTest.cpp"

int main(int argc, char* argv[]){
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
