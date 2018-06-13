#include "AS5601.h"

/**
 *  Constructor of AS5601 class (device address set to ADDRESS)
 *  @param sda  pin name of the I2C data bus
 *  @param scl  pin name of the I2C clock
 */
AS5601::AS5601(PinName sda, PinName scl) : EncoderI2C(ADDRESS,sda,scl) {
    cycle_count = 0;
    past_degrees = 0.0;
}

/**
 *  Constructor of AS5601 class
 *  @param device_address   address of the device
 *  @param sda  pin name of the I2C data bus
 *  @param scl  pin name of the I2C clock
 */
AS5601::AS5601(int device_address, PinName sda, PinName scl) : EncoderI2C(device_address,sda,scl) {
    cycle_count = 0;
    past_degrees = 0.0;
}

/**
 *  Read the angle's bit
 *  @return     angle's bit read
 */
unsigned int AS5601::get_angles_bit() {
    int msb=0, lsb=0;
    device.read_reg8(ANGLE_REG_MSB,&msb);
    device.read_reg8(ANGLE_REG_LSB,&lsb);

    // create the complete number by shifting 8 bit left the msb part
    int bits = (msb<<8 | lsb);
    
    return bits;
}


// TODO
/**
 *  Set the angle's zero position
 */
void AS5601::reset_angle() {
    //I2C_bus_write8(indirizzo,indice_azzera,comando_per_azzerare);
    //device.write_reg8(indice
}

/**
 *  Read the number of bits of the angle
 */
int AS5601::get_resolution_bits() {
    return BIT_ANGLE_RESOLUTION;
}

/**
 *  Read the angle in degrees considering the compensation
 */
float AS5601::get_angle_degrees_with_compensation() {
    float angle_bits = get_angles_bit();
    float bits_resolution=pow(2.0,(double)get_resolution_bits());
    float actual_degrees = (360.0/bits_resolution)*angle_bits;
    
    if(past_degrees > 300 && actual_degrees < 60) {
        cycle_count += 1;
    } else if(past_degrees < 60 && actual_degrees > 300) {
        cycle_count -= 1;
    }
    
    past_degrees = actual_degrees;
    
    return 360*cycle_count+actual_degrees;
}

/**
 *  Read the angle in degrees
 */
float AS5601::get_angle_degrees() {
    float angle_bits = get_angles_bit();
    float bits_resolution=pow(2.0,(double)get_resolution_bits());
    return (360.0/bits_resolution)*angle_bits;
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
void AS5601::write_ABN(unsigned int rate) {
    device.write_reg8(ABN_REGISTER,rate);
}

/**
 *  Read the ABN register
 */
int AS5601::read_ABN() {
    int data = 0;
    device.read_reg8(ABN_REGISTER,&data);
    return data;
}

/**
 *  Read the AGC register
 */
int AS5601::read_AGC() {
    int data = 0;
    device.read_reg8(AGC_REGISTER,&data);
    return data;
}

/**
 *  Return the address of the device
 */
int AS5601::get_address() {
    return ADDRESS;   
}
