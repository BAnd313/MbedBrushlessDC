#include "DigitalEncoderPIC.h"

/**
    * Constructor
    * @param sda - mbed pin to use for the SDA I2C line.
    * @param scl - mbed pin to use for the SCL I2C line.
*/
DigitalEncoderPIC::DigitalEncoderPIC(PinName sda, PinName scl) : DigitalEncoderI2C(PIC_ADDRESS,sda,scl) {
    past_degrees = 0.0;
    cycle_count = 0;
}

/**
    * Constructor
    * @param device_address - address of the I2C device
    * @param sda - mbed pin to use for the SDA I2C line.
    * @param scl - mbed pin to use for the SCL I2C line.
*/
DigitalEncoderPIC::DigitalEncoderPIC(int device_address, PinName sda, PinName scl) : DigitalEncoderI2C(device_address,sda,scl) {
    past_degrees = 0.0;
    cycle_count = 0;
}

/**
 *  Read the integer angle (from 0 to 2^BIT_ANGLE_RESOLUTION)
 *  @return Angle read
 */
int DigitalEncoderPIC::getAngle() {
    int msb=0, lsb=0;
    device.read_reg8(ANGLE_REG_MSB,&msb);
    device.read_reg8(ANGLE_REG_LSB,&lsb);

    // create the complete number by shifting 8 bit left the msb part
    int bits = (msb<<8 | lsb);
    
    return bits - offset;
}

/**
 *  Get the max number of the integer angle
 *  @return Max angle
 */
int DigitalEncoderPIC::getMaxAngle() {
    return pow(2.0,getResolutionBits()) * getGearRatio();
}

/**
 *  Get the resolution of the encoder (in bits)
 *  @return Number of bits of the angle
 */
int DigitalEncoderPIC::getResolutionBits() {
    int bits = 0;
    device.read_reg8(BIT_ANGLE_RESOLUTION_REG,&bits);
    
    return bits;
}

/**
 *  Return the address of the device
 */
int DigitalEncoderPIC::getAddress() {
    return PIC_ADDRESS;   
}
