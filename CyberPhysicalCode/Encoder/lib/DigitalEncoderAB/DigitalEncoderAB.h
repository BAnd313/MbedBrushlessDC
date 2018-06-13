/*
    This library uses TIM2.
    A channel is on PA0
    B channel is on PA1

    Set gearRatio and cpr according to the encoder you are using.
    If you don't have a gear box, set gearRatio to 1 (default value).

    Encoder resolution = gearRatio * countPerRevolution
*/

#include "mbed.h"
#include "DigitalEncoder.h"

#ifndef DIGITAL_ENCODER_AB_H
#define DIGITAL_ENCODER_AB_H

class DigitalEncoderAB : public DigitalEncoder {
    public:
        DigitalEncoderAB(float cpr, float gearRatio = 1.0f);

        int getAngle();
        int getMaxAngle();
        
        void setCountPerRevolution(float cpr);
        float getCountPerRevolution();

        void reset();
    private:
        float cpr;
        void EncoderInitialise_TIM2();
};

#endif
