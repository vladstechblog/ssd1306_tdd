extern "C"
{

#include "hardware/i2c.h"
#include "SSD1306.h"
#include "CppUTestExt/Mocksupport_c.h"


int i2c_write_blocking(i2c_inst_t* i2c, uint8_t addr, const uint8_t* src, size_t len, bool nostop){
  return mock_c()->actualCall("i2c_write_blocking")
    ->withPointerParameters("i2c_instance", i2c)
    ->withUnsignedIntParameters("i2c_addr", addr)
    ->withMemoryBufferParameter("src_buffer", (unsigned char*)src, len)
    ->withUnsignedIntParameters("buff_size", len)
    ->withBoolParameters("nostop", nostop)
    ->returnValue().value.intValue;
}


}//ennd extern "C"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/Mocksupport.h"

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

TEST_GROUP(Commands)
{
  i2c_inst_t* fake_i2c_address = (i2c_inst_t*) 0x11223344;
  void setup(){
    SSD1306_Create(fake_i2c_address);
  }
  void teardown(){
    SSD1306_Destroy();
    mock().clear();
  }
};

TEST(Commands, SendSingleByteCommand){
  unsigned char expected_command_buffer[2] = {
    DISPLAY_CONTROL_CMD_BYTE,
    DISPLAY_CMD_POWER_ON
  };

  unsigned int i2c_addr = DISPLAY_I2C_ADDRESS;
  unsigned int buff_size = sizeof(expected_command_buffer);
  bool nostop = false;

  mock().expectOneCall("i2c_write_blocking")
    .withPointerParameter("i2c_instance", fake_i2c_address)
    .withUnsignedIntParameter("i2c_addr", i2c_addr)
    .withMemoryBufferParameter("src_buffer", (const unsigned char*)expected_command_buffer, buff_size)
    .withUnsignedIntParameter("buff_size", buff_size)
    .withBoolParameter("nostop", nostop)
    .andReturnValue(buff_size);

  SSD1306_sendSingleByteCommand(DISPLAY_CMD_POWER_ON);
  mock().checkExpectations();
}
