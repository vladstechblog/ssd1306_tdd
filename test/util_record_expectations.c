void record_i2c_write_blocking_expectation(const unsigned char* data, size_t length){
  mock().expectOneCall("i2c_write_blocking")
    .withPointerParameter("i2c_instance", fake_i2c_address)
    .withUnsignedIntParameter("i2c_addr", DISPLAY_I2C_ADDRESS)
    .withMemoryBufferParameter("src_buffer", data, length)
    .withUnsignedIntParameter("buff_size", length)
    .withBoolParameter("nostop", false)
    .andReturnValue(length);
}
