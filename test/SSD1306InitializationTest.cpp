extern "C"
{

#include "hardware/i2c.h"
#include "pico/error.h"
#include "SSD1306.h"
#include "CppUTestExt/Mocksupport_c.h"

}//ennd extern "C"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/Mocksupport.h"

#define I2C_CHUNK_SIZE 2

TEST_GROUP(Initialization){
  void record_i2c_write_blocking_expectation(const unsigned char* data, size_t length){
    mock().expectOneCall("i2c_write_blocking")
      .withPointerParameter("i2c_instance", fake_i2c_address)
      .withUnsignedIntParameter("i2c_addr", DISPLAY_I2C_ADDRESS)
      .withMemoryBufferParameter("src_buffer", data, length)
      .withUnsignedIntParameter("buff_size", length)
      .withBoolParameter("nostop", false)
      .andReturnValue(length);
  }

  int expectCommandWithoutSettings(command_t* command){ 
    record_i2c_write_blocking_expectation(command, I2C_CHUNK_SIZE);
    return I2C_CHUNK_SIZE;
  }

  int expectCommandAndOneSetting(command_t* command, setting_t* setting){ 
    record_i2c_write_blocking_expectation(command, I2C_CHUNK_SIZE);
    record_i2c_write_blocking_expectation(setting, I2C_CHUNK_SIZE);
    return I2C_CHUNK_SIZE * 2; 
  }

  int expectCommandAndTwoSettings(command_t* command, setting_t* one, setting_t* two){ 
    record_i2c_write_blocking_expectation(command, I2C_CHUNK_SIZE);
    record_i2c_write_blocking_expectation(one, I2C_CHUNK_SIZE);
    record_i2c_write_blocking_expectation(two, I2C_CHUNK_SIZE);
    return I2C_CHUNK_SIZE * 3;
  }

  void setup(){
    SSD1306_Create(fake_i2c_address);
  }

  void teardown(){
    SSD1306_Destroy();
    mock().checkExpectations();
    mock().clear();
  }
};

TEST(Initialization, DefaultInitialization){
  int expected_bytes_sent = 0;

  command_t mux_cmd_buffer[] = {
    DISPLAY_CONTROL_CMD_BYTE,
    DISPLAY_CMD_SET_MUX_RATIO};
  
  setting_t mux_set_buffer[] = {
    DISPLAY_CONTROL_CMD_BYTE,
    DISPLAY_SETTING_DEFAULT_MUX_RATIO};
  
  expected_bytes_sent = expectCommandAndOneSetting(mux_cmd_buffer, mux_set_buffer);

  command_t disp_offset_cmd_buffer[] = {
    DISPLAY_CONTROL_CMD_BYTE,
    DISPLAY_CMD_SET_VERT_OFFSET};
  
  setting_t disp_offset_set_buffer[] = {
    DISPLAY_CONTROL_CMD_BYTE,
    0};

  expected_bytes_sent += expectCommandAndOneSetting(
      disp_offset_cmd_buffer,
      disp_offset_set_buffer);

  setting_t start_line = 0;
  command_t start_line_cmd = DISPLAY_CMD_SET_START_LINE | start_line;

  command_t start_line_cmd_buffer[] = {
    DISPLAY_CONTROL_CMD_BYTE,
    start_line_cmd};

  expected_bytes_sent += expectCommandWithoutSettings(start_line_cmd_buffer);

  command_t invert_columns_cmd_buffer[] = {
    DISPLAY_CONTROL_CMD_BYTE,
    DISPLAY_CMD_SET_SEGMENT_REMAP
  };
  
  expected_bytes_sent += expectCommandWithoutSettings(invert_columns_cmd_buffer);

  LONGS_EQUAL(expected_bytes_sent, SSD1306_defaultInitialization());
}


