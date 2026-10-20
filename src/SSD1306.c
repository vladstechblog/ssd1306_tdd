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
  int bytes_sent = 0;

  unsigned char data_buffer[] = {DISPLAY_CONTROL_DATA_BYTE, byte_to_send};
  return i2c_write_blocking(i2c_istance_ptr, DISPLAY_I2C_ADDRESS, data_buffer, 2, false);
}
