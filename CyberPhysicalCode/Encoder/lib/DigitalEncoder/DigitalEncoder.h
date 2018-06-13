#include "math.h"

#ifndef DIGITAL_ENCODER_H
#define DIGITAL_ENCODER_H

/**
 *  Abstract class used to implement different types of encoder
 */

enum AngleFormat {angleContinuous = 0, angleMod = 1};

class DigitalEncoder {
    public:
        virtual int getAngle() = 0;
        virtual int getMaxAngle() = 0;
        
        double getAngleDeg();
        double getAngleRad();

        AngleFormat getAngleFormat();
        void setAngleFormat(AngleFormat angleFormat); 

        virtual void reset() = 0;
        
        void setGearRatio(float gr);
        float getGearRatio();
    private:
        AngleFormat angleFormat = angleContinuous;
        int rev_count;
        float gearRatio;
};

#endif
