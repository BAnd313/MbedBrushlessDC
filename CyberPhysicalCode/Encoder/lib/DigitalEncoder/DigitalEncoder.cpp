#include "DigitalEncoder.h"

/**
 *  Get the angle in degrees (from 0° to 360° in angleContinuous mode)
 *  @return The angle in degrees
 */
double DigitalEncoder::getAngleDeg() {
    if(this->angleFormat == angleContinuous) {
        return ((double)getAngle()/getMaxAngle())*360.0f;
    } else if(this->angleFormat == angleMod) {
        int angle = getAngle();
        if(angle>=0) {
            while(angle - getMaxAngle() > 0) {
                angle -= getMaxAngle();
            }
        } else {
            while(angle < 0) {
                angle += getMaxAngle();
            }
        }
        return ((double)angle/getMaxAngle())*360.0f;
    }
}

/**
 *  Get the angle in radiants (from 0 to 2*PI in angleContinuous mode)
 *  @return The angle in radiants
 */
double DigitalEncoder::getAngleRad() {
    if(this->angleFormat == angleContinuous) {
        return ((double)getAngle()/getMaxAngle())*2.0f*M_PI;
    } else if(this->angleFormat == angleMod) {
        int angle = getAngle();
        if(angle>=0) {
            while(angle - getMaxAngle() > 0) {
                angle -= getMaxAngle();
            }
        } else {
            while(angle < 0) {
                angle += getMaxAngle();
            }
        }
        return ((double)angle/getMaxAngle())*2.0f*M_PI;
    }
}

/**
 *  Get the format of the angle
 *  @return The angle format
 */
AngleFormat DigitalEncoder::getAngleFormat() {
    return this->angleFormat;
}   

/**
 *  Set the format of the angle
 */        
void DigitalEncoder::setAngleFormat(AngleFormat angleFormat) {
    this->angleFormat = angleFormat;
}

/**
 *  Set the gear ratio
 */
void DigitalEncoder::setGearRatio(float gr) {
    gearRatio = gr;
}

/**
 *  Get the gear ratio set for the encoder
 *  @return Gear ratio
 */
float DigitalEncoder::getGearRatio() {
    return gearRatio;
}
