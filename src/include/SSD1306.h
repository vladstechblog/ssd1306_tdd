#ifndef D_SSD1306_H
#define D_SSD1306_H

void SSD1306_Create(i2c_inst_t*);
void SSD1306_Destroy(void);
i2c_inst_t* SSD1306_getDisplayAddress(void);

#endif  /* D_SSD1306_H */
