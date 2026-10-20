#ifndef FAKE_I2C_H
#define FAKE_I2C_H

#include <stdint.h>

typedef unsigned int i2c_inst_t; 
int i2c_write_blocking(i2c_inst_t* i2c, uint8_t addr, const uint8_t* src, size_t len, bool nostop);

#endif
