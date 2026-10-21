#include <stdbool.h>
#include "hardware/i2c.h"
#include "pico/error.h"
#include "SSD1306.h"


static i2c_inst_t* i2c_istance_ptr;

void SSD1306_Create(i2c_inst_t* new_i2c_pointer){ i2c_istance_ptr = new_i2c_pointer; }
void SSD1306_Destroy(void){ i2c_istance_ptr = NULL; }
i2c_inst_t* SSD1306_getDisplayAddress(void){ return i2c_istance_ptr; }


int SSD1306_sendSingleByteCommand(command_t command_to_send){
  if (i2c_istance_ptr == NULL)
    return PICO_ERROR_INVALID_ADDRESS;

  if (command_to_send == 0)
    return PICO_ERROR_INVALID_ARG;

  unsigned char command_buffer[] = {DISPLAY_CONTROL_CMD_BYTE, command_to_send};
  return i2c_write_blocking(i2c_istance_ptr, DISPLAY_I2C_ADDRESS, command_buffer, 2, false);
}


int SSD1306_sendCommandWithOneSetting(command_t command, setting_t setting){
  if (command == 0 )
    return PICO_ERROR_INVALID_ARG;

  int bytes_sent = 0;
  bytes_sent += SSD1306_sendSingleByteCommand(command);
  bytes_sent += SSD1306_sendSingleByteCommand(setting);

  return bytes_sent;
}


int SSD1306_sendCommandWithTwoSettings(command_t command, setting_t one, setting_t two){
  if (command == 0 )
    return PICO_ERROR_INVALID_ARG;

  int bytes_sent = 0;
  bytes_sent += SSD1306_sendSingleByteCommand(command);
  bytes_sent += SSD1306_sendSingleByteCommand(one);
  bytes_sent += SSD1306_sendSingleByteCommand(two);

  return bytes_sent;
}


int SSD1306_sendDataByte(data_t byte_to_send){
   if(i2c_istance_ptr == NULL)
     return PICO_ERROR_INVALID_ADDRESS;

  int bytes_sent = 0;

  unsigned char data_buffer[] = {DISPLAY_CONTROL_DATA_BYTE, byte_to_send};
  return i2c_write_blocking(i2c_istance_ptr, DISPLAY_I2C_ADDRESS, data_buffer, 2, false);
}


int SSD1306_setMultiplexRatio(setting_t ratio_to_set){
  if (ratio_to_set < DISPLAY_MUX_RATIO_MIN || ratio_to_set > DISPLAY_MUX_RATIO_MAX)
    return PICO_ERROR_INVALID_ARG;

  return SSD1306_sendCommandWithOneSetting(DISPLAY_CMD_SET_MUX_RATIO, ratio_to_set);
}


int SSD1306_setVerticalOffset(setting_t offset_value){
  if(offset_value >=0 && offset_value <= DISPLAY_ROW_MAX)
    return SSD1306_sendCommandWithOneSetting(DISPLAY_CMD_SET_VERT_OFFSET, offset_value);
  
  return PICO_ERROR_INVALID_ARG;
}

int SSD1306_setDisplayStartLine(setting_t start_line){
  if(start_line >=0 && start_line <= DISPLAY_ROW_MAX)
    return SSD1306_sendSingleByteCommand(DISPLAY_CMD_SET_START_LINE | start_line); 

  return PICO_ERROR_INVALID_ARG;
}

int SSD1306_invertColumnMapping(bool invert_columns){
  unsigned char invert_columns_command = DISPLAY_CMD_SET_SEGMENT_REMAP; 
  if(invert_columns)
    invert_columns_command |= 0x1;

  return SSD1306_sendSingleByteCommand(invert_columns_command);
}

int SSD1306_invertRowMapping(bool invert_rows){
  unsigned char invert_rows_command= DISPLAY_CMD_SET_COM_OUTPUT_DIR; 
  if(invert_rows)
    invert_rows_command |= DISPLAY_SETTING_FLIP_ROWS;

  return SSD1306_sendSingleByteCommand(invert_rows_command);
}

static int send_pin_config(setting_t valid_pin_setting){
  return SSD1306_sendCommandWithOneSetting(DISPLAY_CMD_CONFIGURE_COM_PINS, valid_pin_setting);
}

int SSD1306_configureComPin(setting_t pin_setting){
  switch(pin_setting){
    case DISPLAY_SETTING_COM_PIN_CONFIG_CLEAR:
      return send_pin_config(pin_setting);
    default:
      return PICO_ERROR_INVALID_ARG;
  }//end switch
}
