#include <stdbool.h>
#include "hardware/i2c.h"
#include "SSD1306.h"


static i2c_inst_t* i2c_istance_ptr;

void SSD1306_Create(i2c_inst_t* new_i2c_pointer){ i2c_istance_ptr = new_i2c_pointer; }
void SSD1306_Destroy(void){ i2c_istance_ptr = NULL; }
i2c_inst_t* SSD1306_getDisplayAddress(void){ return i2c_istance_ptr; }

int SSD1306_sendSingleByteCommand(const unsigned char command_to_send){
  unsigned char command_buffer[] = {
    DISPLAY_CONTROL_CMD_BYTE,
    command_to_send
  };

  return i2c_write_blocking(i2c_istance_ptr, DISPLAY_I2C_ADDRESS, command_buffer, 2, false);
}
