#ifndef D_SSD1306_H
#define D_SSD1306_H

#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_CONTROL_CMD_BYTE 0x80
#define DISPLAY_CONTROL_DATA_BYTE 0xC0

#define DISPLAY_CMD_POWER_ON 0xAF
#define DISPLAY_CMD_SET_CONTRAST 0x7F
#define DISPLAY_CMD_SET_PAGE_ADDR 0x22

typedef const unsigned char command_t;
typedef const unsigned char setting_t;
typedef const unsigned char data_t;


void SSD1306_Create(i2c_inst_t*);
void SSD1306_Destroy(void);
i2c_inst_t* SSD1306_getDisplayAddress(void);
int SSD1306_sendSingleByteCommand(command_t);
int SSD1306_sendCommandWithOneSetting(command_t, setting_t);
int SSD1306_sendCommandWithTwoSettings(command_t, setting_t, setting_t);
int SSD1306_sendDataByte(data_t);

#endif  /* D_SSD1306_H */
