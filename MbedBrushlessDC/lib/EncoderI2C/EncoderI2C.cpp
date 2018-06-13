#include "EncoderI2C.h"

/**
 *  Constructor of EncoderI2C class
 *  @param device_address   address of the device (default 0)
 *  @param sda  pin name of the I2C data bus (default I2C_SDA)
 *  @param scl  pin name of the I2C clock (default I2C_SCL)
 */
EncoderI2C::EncoderI2C(int device_address = 0x0, PinName sda = I2C_SDA, PinName scl = I2C_SCL) : device(device_address,sda,scl){
    
}