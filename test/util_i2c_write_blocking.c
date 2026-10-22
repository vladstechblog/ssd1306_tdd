#include <stdint.h>
#include <stdbool.h>
#include "CppUTestExt/Mocksupport_c.h"
#include "hardware/i2c.h"

int i2c_write_blocking(i2c_inst_t* i2c, uint8_t addr, const uint8_t* src, size_t len, bool nostop){
  return mock_c()->actualCall("i2c_write_blocking")
    ->withPointerParameters("i2c_instance", i2c)
    ->withUnsignedIntParameters("i2c_addr", addr)
    ->withMemoryBufferParameter("src_buffer", (unsigned char*)src, len)
    ->withUnsignedIntParameters("buff_size", len)
    ->withBoolParameters("nostop", nostop)
    ->returnValue().value.intValue;
}
