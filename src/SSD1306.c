#include "hardware/i2c.h"
#include "SSD1306.h"

static i2c_inst_t* i2c_istance_ptr;

void SSD1306_Create(i2c_inst_t* new_i2c_pointer){ i2c_istance_ptr = new_i2c_pointer; }
void SSD1306_Destroy(void){ i2c_istance_ptr = NULL; }

i2c_inst_t* SSD1306_getDisplayAddress(void){ return i2c_istance_ptr; }
