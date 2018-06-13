#include "mbed.h"
#include "math.h"
#include "EncoderI2C.h"

#ifndef AS5601_H
#define AS5601_H

class AS5601 : public EncoderI2C {
    public:
        AS5601(PinName sda, PinName scl);
        AS5601(int device_address, PinName sda, PinName scl);
        void write_ABN(unsigned int rate);
        int read_ABN();
        int read_AGC();
        virtual void reset_angle();
        virtual float get_angle_degrees();
        virtual int get_address();
        float get_angle_degrees_with_compensation();
        
    private: 
        virtual unsigned int get_angles_bit();
        virtual int get_resolution_bits();
        
        /// Variables used for angle compensation
        double past_degrees;
        int cycle_count;
        
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