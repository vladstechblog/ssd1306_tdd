#ifndef D_SSD1306_H
#define D_SSD1306_H

#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH 128
#define DISPLAY_ROW_MAX (DISPLAY_HEIGHT -1)
#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_CONTROL_CMD_BYTE 0x80
#define DISPLAY_CONTROL_DATA_BYTE 0xC0

#define DISPLAY_MUX_RATIO_MIN 15
#define DISPLAY_MUX_RATIO_MAX 63
#define DISPLAY_START_LINE_MAX 0x7F //per datasheet

#define DISPLAY_SETTING_FLIP_ROWS 0x8
#define DISPLAY_SETTING_COM_PIN_CONFIG_SEQ 0x2
#define DISPLAY_SETTING_COM_PIN_CONFIG_ALT 0x12
#define DISPLAY_SETTING_COM_PIN_LEFT_RIGHT_REMAP_ON 0x22
#define DISPLAY_SETTING_MAX_CONTRAST 0x7F

#define DISPLAY_CMD_POWER_ON 0xAF
#define DISPLAY_CMD_SET_CONTRAST 0x7F
#define DISPLAY_CMD_SET_PAGE_ADDR 0x22
#define DISPLAY_CMD_SET_MUX_RATIO 0xA8
#define DISPLAY_CMD_SET_VERT_OFFSET 0xD3
#define DISPLAY_CMD_SET_START_LINE 0x40
#define DISPLAY_CMD_SET_SEGMENT_REMAP 0xA0
#define DISPLAY_CMD_SET_COM_OUTPUT_DIR 0xC0
#define DISPLAY_CMD_CONFIGURE_COM_PINS 0xDA
#define DISPLAY_CMD_SET_ENTIRE_ON 0xA4

typedef unsigned char command_t;
typedef unsigned char setting_t;
typedef unsigned char data_t;


void SSD1306_Create(i2c_inst_t*);
void SSD1306_Destroy(void);
i2c_inst_t* SSD1306_getDisplayAddress(void);
int SSD1306_sendSingleByteCommand(command_t);
int SSD1306_sendCommandWithOneSetting(command_t, setting_t);
int SSD1306_sendCommandWithTwoSettings(command_t, setting_t, setting_t);
int SSD1306_sendDataByte(data_t);
int SSD1306_setMultiplexRatio(setting_t);
int SSD1306_setVerticalOffset(setting_t);
int SSD1306_setDisplayStartLine(setting_t);
int SSD1306_invertColumnMapping(bool);
int SSD1306_invertRowMapping(bool);
int SSD1306_configureComPin(setting_t);
int SSD1306_setContrast(setting_t);
int SSD1306_setEntireDisplayOn(bool);

#endif  /* D_SSD1306_H */
