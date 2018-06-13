#include "I2CRW.h"
#include "DigitalEncoder.h"

#ifndef DIGITAL_ENCODER_I2C_H
#define DIGITAL_ENCODER_I2C_H

/**
 *  Abstract class used to implement different types of I2C encoder
 */
class DigitalEncoderI2C : public DigitalEncoder {
    public:
        DigitalEncoderI2C(int device_address, PinName sda, PinName scl);
        virtual int getAngle() = 0;
        virtual int getMaxAngle() = 0;
        virtual int getAddress() = 0;
        
        void reset();
    protected:
        I2CRW device;
        int offset;
};

#endif
