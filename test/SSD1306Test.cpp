extern "C"
{

#include "hardware/i2c.h"
#include "SSD1306.h"

}

#include "CppUTest/TestHarness.h"

TEST_GROUP(SSD1306)
{
  i2c_inst_t* fake_i2c_address = (i2c_inst_t*) 0x11223344;
  void setup(){
    SSD1306_Create(fake_i2c_address);
  }
  void teardown(){}
};

TEST(SSD1306, Create){
  POINTERS_EQUAL(fake_i2c_address, SSD1306_getDisplayAddress());
}


TEST(SSD1306, Destroy){
  SSD1306_Destroy();
  POINTERS_EQUAL(NULL, SSD1306_getDisplayAddress());
}


