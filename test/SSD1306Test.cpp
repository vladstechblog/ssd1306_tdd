extern "C"
{

#include "hardware/i2c.h"
#include "SSD1306.h"
#include "CppUTestExt/Mocksupport_c.h"

#include "mock_call_implementations.c"

}//ennd extern "C"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/Mocksupport.h"

TEST_GROUP(SSD1306)
{
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
  void setup(){
    SSD1306_Create(fake_i2c_address);
  }
  void teardown(){
    SSD1306_Destroy();
    mock().checkExpectations();
    mock().clear();
  }

  void record_i2c_write_blocking_expectation(const unsigned char* data, size_t length){
    mock().expectOneCall("i2c_write_blocking")
      .withPointerParameter("i2c_instance", fake_i2c_address)
      .withUnsignedIntParameter("i2c_addr", DISPLAY_I2C_ADDRESS)
      .withMemoryBufferParameter("src_buffer", data, length)
      .withUnsignedIntParameter("buff_size", length)
      .withBoolParameter("nostop", false)
      .andReturnValue(length);
   }
};

TEST(Commands, SendSingleByteCommand){
  unsigned char expected_command_buffer[2] = {
    DISPLAY_CONTROL_CMD_BYTE,
    DISPLAY_CMD_POWER_ON
  };

  unsigned int i2c_addr = DISPLAY_I2C_ADDRESS;
  size_t expected_size = sizeof(expected_command_buffer);

  record_i2c_write_blocking_expectation(expected_command_buffer, expected_size);
  int return_value = SSD1306_sendSingleByteCommand(DISPLAY_CMD_POWER_ON);
  LONGS_EQUAL(return_value, expected_size);
}

