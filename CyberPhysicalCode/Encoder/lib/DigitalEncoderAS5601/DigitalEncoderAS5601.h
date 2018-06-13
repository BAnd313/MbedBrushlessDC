#include "DigitalEncoderI2C.h"

#ifndef DIGITAL_ENCODER_AS5601_H
#define DIGITAL_ENCODER_AS5601_H

class DigitalEncoderAS5601 : public DigitalEncoderI2C {
    public:
        DigitalEncoderAS5601(PinName sda, PinName scl);
        DigitalEncoderAS5601(int device_address, PinName sda, PinName scl);

        void write_register_ABN(unsigned int rate);
        int read_register_ABN();
        int read_register_AGC();

        int getAngle();
        int getMaxAngle();
        int getAddress();
        
    private:        
        /// Address of the device
        static const int ADDRESS = 0x36;
        
        /// Number of bit read for the angle
        static const int BIT_ANGLE_RESOLUTION = 12;
        
        /// Address of ABN register
        static const int ABN_REGISTER = 0x09;
        
        /// Address of raw angle register (MSB)
        static const int RAW_ANGLE_REG_MSB = 0x0C;
        
        /// Address of raw angle register (LSB)
        static const int RAW_ANGLE_REG_LSB = 0x0D;
        
        /// Address of angle register (MSB)
        static const int ANGLE_REG_MSB = 0x0E;
        
        /// Address of angle register (LSB)
        static const int ANGLE_REG_LSB = 0x0F;
        
        /// Address of AGC register
        static const int AGC_REGISTER = 0x1A;
};

#endif
