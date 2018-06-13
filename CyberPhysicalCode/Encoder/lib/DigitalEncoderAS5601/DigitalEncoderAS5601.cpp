#include "DigitalEncoderAS5601.h"

/**
 *  Constructor of DigitalEncoderAS5601 class (device address set to ADDRESS)
 *  @param sda  pin name of the I2C data bus
 *  @param scl  pin name of the I2C clock
 */
DigitalEncoderAS5601::DigitalEncoderAS5601(PinName sda, PinName scl) : DigitalEncoderI2C(ADDRESS,sda,scl) {
    
}

/**
 *  Constructor of DigitalEncoderAS5601 class
 *  @param device_address   address of the device
 *  @param sda  pin name of the I2C data bus
 *  @param scl  pin name of the I2C clock
 */
DigitalEncoderAS5601::DigitalEncoderAS5601(int device_address, PinName sda, PinName scl) : DigitalEncoderI2C(device_address,sda,scl) {
    
}

/**
 *  Read the integer angle (from 0 to 2^BIT_ANGLE_RESOLUTION)
 *  @return Angle read
 */
int DigitalEncoderAS5601::getAngle() {
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
int DigitalEncoderAS5601::getMaxAngle() {
    return pow(2.0,BIT_ANGLE_RESOLUTION) * getGearRatio();
}

/* Set the ABN register
 * @param   rate 
 * 0000 : 8 (61 Hz)
 * 0001 : 16 (122 Hz)
 * 0010 : 32 (244 Hz)
 * 0011 : 64 (488 Hz)
 * 0100 : 128 (976 Hz) 
 * 0101 : 256 (1.9 kHz) 
 * 0110 : 512 (3.9 kHz) 
 * 0111 : 1024 (7.8 kHz) 
 * others : 2048 (15.6 kHz)
 */
void DigitalEncoderAS5601::write_register_ABN(unsigned int rate) {
    device.write_reg8(ABN_REGISTER,rate);
}

/**
 *  Read the ABN register
 */
int DigitalEncoderAS5601::read_register_ABN() {
    int data = 0;
    device.read_reg8(ABN_REGISTER,&data);
    return data;
}

/**
 *  Read the AGC register
 */
int DigitalEncoderAS5601::read_register_AGC() {
    int data = 0;
    device.read_reg8(AGC_REGISTER,&data);
    return data;
}

/**
 *  Return the address of the device
 */
int DigitalEncoderAS5601::getAddress() {
    return ADDRESS;   
}
