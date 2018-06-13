#include "mbed.h"
#include "I2CRW.h"

#ifndef ENCODER_I2C_H
#define ENCODER_I2C_H

/**
 *  Abstract class used to implement different types of I2C encoder
 */
class EncoderI2C {
    public:
        EncoderI2C(int device_address, PinName sda, PinName scl);
        virtual void reset_angle() = 0;
        virtual float get_angle_degrees() = 0;
        virtual int get_address() = 0;
        
    protected:
        I2CRW device;
        virtual unsigned int get_angles_bit() = 0;
        virtual int get_resolution_bits() = 0;
};

#endif