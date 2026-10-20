extern "C"
{

#include "hardware/i2c.h"
#include "pico/error.h"
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

TEST(Commands, SingleNullByteReturnPicoInvalidArgCode){
  unsigned char expected_command_buffer[2] = { DISPLAY_CONTROL_CMD_BYTE, 0 };
  int return_value = SSD1306_sendSingleByteCommand(0);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, return_value);
}

TEST(Commands, InitializationWithNullRerturnsInvalidAddressError){
  SSD1306_Create(0);
  int return_value = SSD1306_sendSingleByteCommand(DISPLAY_CMD_POWER_ON);
  LONGS_EQUAL(PICO_ERROR_INVALID_ADDRESS, return_value);
}

TEST(Commands, SendCommandWithOneSetting){
  unsigned char command = DISPLAY_CMD_SET_CONTRAST;
  unsigned char setting = 0xFF;
  unsigned char command_buffer[] = {DISPLAY_CONTROL_CMD_BYTE, command};
  unsigned char setting_buffer[] = {DISPLAY_CONTROL_CMD_BYTE, setting};

  record_i2c_write_blocking_expectation(command_buffer, sizeof(command_buffer));
  record_i2c_write_blocking_expectation(setting_buffer, sizeof(setting_buffer));
  int return_value =  SSD1306_sendCommandWithOneSetting(command, setting);
}

TEST(Commands, CommandNullByteReturnPicoInvalidArgCode){
  int return_value =  SSD1306_sendCommandWithOneSetting(0, 0xff);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, return_value);
}

TEST(Commands, SendCommandWithTwoSettings){
  unsigned char command = DISPLAY_CMD_SET_PAGE_ADDR;
  unsigned char setting_one = 1;
  unsigned char setting_two = 10;

  unsigned char command_buffer[] = {DISPLAY_CONTROL_CMD_BYTE, command};
  unsigned char setting_one_buffer[] = {DISPLAY_CONTROL_CMD_BYTE, setting_one};
  unsigned char setting_two_buffer[] = {DISPLAY_CONTROL_CMD_BYTE, setting_two};

  int expected_total_data_sent =
    sizeof(command_buffer) + sizeof(setting_one_buffer) + sizeof(setting_two_buffer);
  
  record_i2c_write_blocking_expectation(command_buffer, sizeof(command_buffer));
  record_i2c_write_blocking_expectation(setting_one_buffer, sizeof(setting_one_buffer));
  record_i2c_write_blocking_expectation(setting_two_buffer, sizeof(setting_two_buffer));

  int return_value =  SSD1306_sendCommandWithTwoSettings(command, setting_one, setting_two);
  LONGS_EQUAL(expected_total_data_sent, return_value); 
}

TEST(Commands, CommandNullByteReturnPicoInvalidArgCodeForCommandWithTwoSettings){
  int return_value =  SSD1306_sendCommandWithTwoSettings(0, 0xff, 0xff);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, return_value);
}
