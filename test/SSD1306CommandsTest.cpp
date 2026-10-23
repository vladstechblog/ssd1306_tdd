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

static void clear_i2c_command_bytes(command_t i2c_chunk[I2C_CHUNK_SIZE]){
  i2c_chunk[0] = DISPLAY_CONTROL_CMD_BYTE;
  i2c_chunk[1] = 0;
}

TEST_GROUP(Commands){
  command_t command_buffer[I2C_CHUNK_SIZE]     = {DISPLAY_CONTROL_CMD_BYTE, 0x0};
  setting_t setting_one_buffer[I2C_CHUNK_SIZE] = {DISPLAY_CONTROL_CMD_BYTE, 0x0};
  setting_t setting_two_buffer[I2C_CHUNK_SIZE] = {DISPLAY_CONTROL_CMD_BYTE, 0x0};

  void clear_buffers(){
    clear_i2c_command_bytes(command_buffer);
    clear_i2c_command_bytes(setting_one_buffer);
    clear_i2c_command_bytes(setting_two_buffer);
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

  int expectCommandWithoutSettings(command_t command){ 
    command_buffer[1] = command;
    record_i2c_write_blocking_expectation(command_buffer, sizeof(command_buffer));
    return sizeof(command_buffer);
  }

  int expectCommandAndOneSetting(command_t command, setting_t setting){ 
    command_buffer[1] = command;
    setting_one_buffer[1] = setting;
    record_i2c_write_blocking_expectation(command_buffer, sizeof(command_buffer));
    record_i2c_write_blocking_expectation(setting_one_buffer, sizeof(setting_one_buffer));
    return sizeof(command_buffer) + sizeof(setting_one_buffer);
  }

  int expectCommandAndTwoSettings(command_t command, setting_t one, setting_t two){ 
    command_buffer[1] = command;
    setting_one_buffer[1] = one;
    setting_two_buffer[1] = two;
    record_i2c_write_blocking_expectation(command_buffer, sizeof(command_buffer));
    record_i2c_write_blocking_expectation(setting_one_buffer, sizeof(setting_one_buffer));
    record_i2c_write_blocking_expectation(setting_two_buffer, sizeof(setting_two_buffer));
    return sizeof(command_buffer) + sizeof(setting_one_buffer) + sizeof(setting_two_buffer);
  }

  void setup(){
    clear_buffers();
    SSD1306_Create(fake_i2c_address);
  }

  void teardown(){
    SSD1306_Destroy();
    mock().checkExpectations();
    mock().clear();
  }
};

TEST(Commands, SendSingleByteCommand){
  command_t command = DISPLAY_CMD_POWER_ON;
  int expected = expectCommandWithoutSettings(command);
  LONGS_EQUAL(expected, SSD1306_sendSingleByteCommand(command));
}

/*
TEST(Commands, SingleNullByteReturnPicoInvalidArgCode){
  unsigned char expected_command_buffer[2] = { DISPLAY_CONTROL_CMD_BYTE, 0 };
  int return_value = SSD1306_sendSingleByteCommand(0);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, return_value);
}
*/

TEST(Commands, I2CNullPTRGuardRailForSingleByteCommand){
  SSD1306_Create(0);
  int return_value = SSD1306_sendSingleByteCommand(DISPLAY_CMD_POWER_ON);
  LONGS_EQUAL(PICO_ERROR_INVALID_ADDRESS, return_value);
}

TEST(Commands, SendCommandWithOneSetting){
  command_t command = DISPLAY_CMD_SET_CONTRAST;
  setting_t setting = 0xFF;
  int expected = expectCommandAndOneSetting(command, setting);
  LONGS_EQUAL(expected, SSD1306_sendCommandWithOneSetting(command, setting));
}

TEST(Commands, CommandNullByteReturnPicoInvalidArgCode){
  int return_value =  SSD1306_sendCommandWithOneSetting(0, 0xff);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, return_value);
}

TEST(Commands, SendCommandWithTwoSettings){
  command_t command = DISPLAY_CMD_SET_PAGE_ADDR;
  setting_t one = 1;
  setting_t two = 10;
  int expected = expectCommandAndTwoSettings(command, one, two);
  int return_value =  SSD1306_sendCommandWithTwoSettings(command, one, two);
  LONGS_EQUAL(expected, return_value); 
}

TEST(Commands, CommandNullByteReturnPicoInvalidArgCodeForCommandWithTwoSettings){
  int return_value = SSD1306_sendCommandWithTwoSettings(0, 0xff, 0xff);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, return_value);
}

TEST(Commands, SendDataByte){
  data_t byte_to_send = 0xff;
  data_t data_buffer[] = {DISPLAY_CONTROL_DATA_BYTE, byte_to_send};
  int expected_data_length = sizeof(data_buffer);
  record_i2c_write_blocking_expectation(data_buffer, expected_data_length);
  int return_value = SSD1306_sendDataByte(byte_to_send);
  LONGS_EQUAL(expected_data_length, return_value);
}

TEST(Commands, I2CNullPTRGuardRailForSendingDataByte){
  SSD1306_Create(0);
  int return_value = SSD1306_sendDataByte(0xff);
  LONGS_EQUAL(PICO_ERROR_INVALID_ADDRESS, return_value);
}

TEST(Commands, SetMultiplexRatio){
  setting_t expected_ratio = 63;
  int expected = expectCommandAndOneSetting(DISPLAY_CMD_SET_MUX_RATIO, expected_ratio);
  LONGS_EQUAL(expected, SSD1306_setMultiplexRatio(expected_ratio));
}

TEST(Commands, SetMutliplexRatioLessThanMiniumReturnsPicoInvalidArgument){
  int return_value = SSD1306_setMultiplexRatio(DISPLAY_MUX_RATIO_MIN - 1);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, return_value);
}

TEST(Commands, SetMutliplexRatioGreaterThanMaximumReturnsPicoInvalidArgument){
  int return_value = SSD1306_setMultiplexRatio(DISPLAY_MUX_RATIO_MAX + 1);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, return_value);
}

TEST(Commands, SetVerticalOffsetToOne){
  setting_t expected_offset = 1;
  int expected = expectCommandAndOneSetting(DISPLAY_CMD_SET_VERT_OFFSET,expected_offset);
   LONGS_EQUAL(expected, SSD1306_setVerticalOffset(expected_offset));
}

TEST(Commands, SetVerticalOffsetToZero){
  setting_t expected_offset = 0;
  int expected = expectCommandAndOneSetting(DISPLAY_CMD_SET_VERT_OFFSET,expected_offset);
   LONGS_EQUAL(expected, SSD1306_setVerticalOffset(expected_offset));
}

TEST(Commands, VerticalOffsetAboveMaximumReturnPicoErrorInvalidArgument){
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setVerticalOffset(DISPLAY_ROW_MAX + 1));
}

TEST(Commands, VerticalOffsetBelowMinimumReturnPicoErrorInvalidArgument){
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setVerticalOffset(-1));
}

TEST(Commands, SetDisplayStartLine){
  setting_t expected_start_line = 32;
  setting_t test_setting = DISPLAY_CMD_SET_START_LINE | expected_start_line;
  command_t command_buffer[] = { DISPLAY_CONTROL_CMD_BYTE, test_setting};
  int expected_total_data_sent = sizeof(command_buffer);

  record_i2c_write_blocking_expectation(command_buffer, sizeof(command_buffer));
  LONGS_EQUAL(expected_total_data_sent, SSD1306_setDisplayStartLine(expected_start_line));
}

TEST(Commands, StartLineAboveMaximumReturnsPicoErrorInvalidArgument){
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setDisplayStartLine(DISPLAY_HEIGHT));
}

TEST(Commands, StartLineBelowMinimumReturnsPicoErrorInvalidArgument){
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setDisplayStartLine(-1));
}

TEST(Commands, EnbleIvertColumns){
  int expected = expectCommandWithoutSettings(DISPLAY_CMD_SET_SEGMENT_REMAP | 1);
  LONGS_EQUAL(expected, SSD1306_invertColumnMapping(true));
}

TEST(Commands, DisableIvertColumns){
  int expected = expectCommandWithoutSettings(DISPLAY_CMD_SET_SEGMENT_REMAP);
  LONGS_EQUAL(expected, SSD1306_invertColumnMapping(false));
}

TEST(Commands, EnableIvertRows){
  command_t command = DISPLAY_CMD_SET_COM_OUTPUT_DIR | DISPLAY_SETTING_FLIP_ROWS;
  int expected = expectCommandWithoutSettings(command);
  LONGS_EQUAL(expected, SSD1306_invertRowMapping(true));
}

TEST(Commands, DisableIvertRows){
  command_t command_buffer[] = {DISPLAY_CONTROL_CMD_BYTE, DISPLAY_CMD_SET_COM_OUTPUT_DIR};
  record_i2c_write_blocking_expectation(command_buffer, sizeof(command_buffer));
  LONGS_EQUAL(sizeof(command_buffer), SSD1306_invertRowMapping(false));
}

TEST(Commands, SetSequenctialPinConfig){
  setting_t pin_setting = DISPLAY_SETTING_COM_PIN_CONFIG_SEQ;
  int expected = expectCommandAndOneSetting(DISPLAY_CMD_CONFIGURE_COM_PINS, pin_setting);
  LONGS_EQUAL(expected, SSD1306_configureComPin(pin_setting));
}

TEST(Commands, SetAltPinConfig){
  setting_t pin_setting = DISPLAY_SETTING_COM_PIN_CONFIG_ALT;
  int expected = expectCommandAndOneSetting(DISPLAY_CMD_CONFIGURE_COM_PINS, pin_setting);
  LONGS_EQUAL(expected, SSD1306_configureComPin(pin_setting));
}

TEST(Commands, SetLeftRightRemapping){
  setting_t pin_setting = DISPLAY_SETTING_COM_PIN_LEFT_RIGHT_REMAP_ON;
  int expected = expectCommandAndOneSetting(DISPLAY_CMD_CONFIGURE_COM_PINS, pin_setting);
  LONGS_EQUAL(expected, SSD1306_configureComPin(pin_setting));
}

TEST(Commands, SetAlternaltePinConfigAndEnableLeftRightRemapping){
  setting_t pin_setting = 
    DISPLAY_SETTING_COM_PIN_CONFIG_ALT | DISPLAY_SETTING_COM_PIN_LEFT_RIGHT_REMAP_ON;;
  int expected = expectCommandAndOneSetting(DISPLAY_CMD_CONFIGURE_COM_PINS, pin_setting);
  LONGS_EQUAL(expected, SSD1306_configureComPin(pin_setting));
}

TEST(Commands, InvalidPinConfigReturnsPicoErrorInvaidArg){
  setting_t pin_setting = 0b110;
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_configureComPin(pin_setting));
}

TEST(Commands, SetContrast){
  setting_t contrast = DISPLAY_SETTING_MAX_CONTRAST;
  int expected = expectCommandAndOneSetting(DISPLAY_CMD_SET_CONTRAST, contrast);
  LONGS_EQUAL(expected, SSD1306_setContrast(contrast));
}

TEST(Commands, ContrastGreaterThanMaxReturnsPicoErrorInvalidArg){
  setting_t contrast = DISPLAY_SETTING_MAX_CONTRAST+1;
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setContrast(contrast));
}

TEST(Commands, NegativeContrastThanMaxReturnsPicoErrorInvalidArg){
  setting_t contrast = -1; 
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setContrast(contrast));
}

TEST(Commands, SetEntireDisplayOn){
  command_t command = DISPLAY_CMD_SET_ENTIRE_ON;
  int expected = expectCommandWithoutSettings(command);
  LONGS_EQUAL(expected, SSD1306_setEntireDisplayOn(false));
}

TEST(Commands, SetEntireDisplayOnIngoreRamContent){
  command_t command = DISPLAY_CMD_SET_ENTIRE_ON | 0x1;
  int expected = expectCommandWithoutSettings(command);
  LONGS_EQUAL(expected, SSD1306_setEntireDisplayOn(true));
}

TEST(Commands, DoNotInvertDisplayPixels){
  command_t command = DISPLAY_CMD_INVERT_PIXELS;
  int expected = expectCommandWithoutSettings(command);
  LONGS_EQUAL(expected, SSD1306_invertDisplayPixels(false));
}

TEST(Commands, InvertDisplayPixels){
  command_t command = DISPLAY_CMD_INVERT_PIXELS | 0x1;
  int expected = expectCommandWithoutSettings(command);
  LONGS_EQUAL(expected, SSD1306_invertDisplayPixels(true));
}

TEST(Commands, SetDefaultOscillatorFrequencyAndDividerRatio){
  command_t command = DISPLAY_CMD_CONFIGURE_OSCILLATOR;
  setting_t setting = DISPLAY_SETTING_DEFAULT_FREQ_AND_DIVIDER_RATIO;
  int expected = expectCommandAndOneSetting(command, setting);
  LONGS_EQUAL(expected, SSD1306_setDefaultFreqAndDividerRatio());
}

TEST(Commands, DisplayPowerOff){
  command_t power_off = DISPLAY_CMD_POWER_OFF;
  int expected = expectCommandWithoutSettings(power_off);
  LONGS_EQUAL(expected, SSD1306_displayPowerOff());
}

TEST(Commands, DisplayPowerOn){
  command_t power_on = DISPLAY_CMD_POWER_ON;
  int expected = expectCommandWithoutSettings(power_on);
  LONGS_EQUAL(expected, SSD1306_displayPowerOn());
}

TEST(Commands, EnableChargePump){
  command_t charge_pump_command = DISPLAY_CMD_CHARGE_PUMP_SETTINGS;
  setting_t charge_pump_setting = DISPLAY_SETTING_ENABLE_CHARGE_PUMP;
  command_t second_buffer[] = {DISPLAY_CONTROL_CMD_BYTE, DISPLAY_CMD_POWER_ON}; 
  int expected_first_part = expectCommandAndOneSetting(charge_pump_command, charge_pump_setting);
  record_i2c_write_blocking_expectation(second_buffer, sizeof(second_buffer));
  LONGS_EQUAL(expected_first_part + sizeof(second_buffer), SSD1306_enableChargePump());
}

TEST(Commands, SetMemoryAddressingModeHorizontal){
  command_t command = DISPLAY_CMD_SET_MEMORY_ADDRESSING_MODE;
  setting_t setting = DISPLAY_SETTING_ADDR_MODE_HORIZONTAL; 
  int expected = expectCommandAndOneSetting(command, setting);
  LONGS_EQUAL(expected, SSD1306_setAddressModeToHorizontal());
}

TEST(Commands, SetMemoryAddressingModeVertical){
  command_t command = DISPLAY_CMD_SET_MEMORY_ADDRESSING_MODE;
  setting_t setting = DISPLAY_SETTING_ADDR_MODE_VERTICAL; 
  int expected = expectCommandAndOneSetting(command, setting);
  LONGS_EQUAL(expected, SSD1306_setAddressModeToVertical());
}

TEST(Commands, SetMemoryAddressingModePage){
  command_t command = DISPLAY_CMD_SET_MEMORY_ADDRESSING_MODE;
  setting_t setting = DISPLAY_SETTING_ADDR_MODE_PAGE; 
  int expected = expectCommandAndOneSetting(command, setting);
  LONGS_EQUAL(expected, SSD1306_setAddressModeToPage());
}

TEST(Commands, SetPageSartAddress){
  setting_t page_address = 2;
  command_t command = DISPLAY_CMD_SET_PAGE_START_ADDR | page_address;
  int expected = expectCommandWithoutSettings(command);
  LONGS_EQUAL(expected, SSD1306_setPageStartAddress(page_address));
}

TEST(Commands, SetPageSartGreaterThanMaxReturnsPicoErrorInvalidArgument){
  setting_t address = DISPLAY_PAGE_MAX + 1;
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setPageStartAddress(address));
}

TEST(Commands, SetPageSartLessThanMinReturnsPicoErrorInvalidArgument){
  setting_t address = -1;
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setPageStartAddress(address));
}

TEST(Commands, SetPageAddressRange){
  setting_t start = 2;
  setting_t end = 5; 
  command_t command = DISPLAY_CMD_SET_PAGE_RANGE;
  int expected = expectCommandAndTwoSettings(command, start, end);
  LONGS_EQUAL(expected, SSD1306_setPageRange(start, end));
}

TEST(Commands, SetPageRangeEndGreaterThanMaxReturnsPicoErrorInvalidArgument){
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setPageRange(0, DISPLAY_PAGE_MAX +1));
}

TEST(Commands, SetPageRangeEndLessThanMinPicoErrorInvalidArgument){
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setPageRange(0, DISPLAY_PAGE_MIN - 1));
}

TEST(Commands, SetPageRangeStartGreaterThanMaxPicoErrorInvalidArgument){
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setPageRange(DISPLAY_PAGE_MAX + 1, 2));
}

TEST(Commands, SetPageRangeStartLessThanMinPicoErrorInvalidArgument){
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setPageRange(DISPLAY_PAGE_MIN - 1, 2));
}

TEST(Commands, SetPageRangeStartGreaterThanEndReturnsPicoErrorInvalidArgument){
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, SSD1306_setPageRange(3, 2));
}
