#include "DigitalEncoderI2C.h"

#ifndef DIGITAL_ENCODER_PIC_H
#define DIGITAL_ENCODER_PIC_H

class DigitalEncoderPIC : public DigitalEncoderI2C {
    public:
        DigitalEncoderPIC(PinName sda, PinName scl); 
        DigitalEncoderPIC(int device_address, PinName sda, PinName scl);

        int getAngle();
        int getMaxAngle();
        int getAddress();

    private:
        int getResolutionBits();

        //Contains the angle read
        static const int ANGLE_REG_MSB = 0;
        static const int ANGLE_REG_LSB = 1;

        //Contains the resolution bits
        static const int BIT_ANGLE_RESOLUTION_REG = 5;

        // For resetting the angle position
        static const int RESET_COMMAND = 166;  // to write in the register
        static const int RESET_INDEX = 9;    // the register

        // The address of the device
        static const int PIC_ADDRESS = 0x41;

        // Other registers not used 
        static const int MSB_COEFF_INDEX = 6;
        static const int LSB_COEFF_INDEX = 7;

        static const int MSB_REAL_INDEX = 2;
        static const int LSB_REAL_INDEX = 3;

        // variable used for the compensation
        float past_degrees;
        int cycle_count;
};

#endif