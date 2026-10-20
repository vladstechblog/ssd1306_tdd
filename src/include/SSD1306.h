#ifndef D_SSD1306_H
#define D_SSD1306_H

#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_CONTROL_CMD_BYTE 0x80
#define DISPLAY_CMD_POWER_ON 0xAF

void SSD1306_Create(i2c_inst_t*);
void SSD1306_Destroy(void);
i2c_inst_t* SSD1306_getDisplayAddress(void);
int SSD1306_sendSingleByteCommand(const unsigned char);

#endif  /* D_SSD1306_H */
