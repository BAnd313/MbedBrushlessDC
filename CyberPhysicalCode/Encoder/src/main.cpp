#include "mbed.h"
#include "DigitalEncoderAB.h"
#include "DigitalEncoderAS5601.h"
#include "DigitalEncoderPIC.h"

//creating an object of serial class to communicate with PC
Serial pc(SERIAL_TX, SERIAL_RX);
//setting LED1 to give digital output
DigitalOut myled(LED1);

// PIC encoder
//DigitalEncoderPIC encoder(I2C_SDA,I2C_SCL);

// AS5601 encoder
DigitalEncoderAS5601 encoder(PB_9, PB_8);

// AB encoder (PA0 = A, PA1 = B)
// DigitalEncoderAB encoder(48.0f);

int main()
{
    // for printing float values
    asm(".global _printf_float");

    float angle_deg, angle_rad;

    //encoder.setAngleFormat(angleContinuous);

    int count = 0;

    while(1) {
        // read the angle
        angle_deg = encoder.getAngleDeg();
        angle_rad = encoder.getAngleRad();

        pc.printf("%f, %f\n",angle_deg,angle_rad);

        // every 7 seconds reset the angle
        count++;
        if(count>70) {
            count = 0;
            encoder.reset();
        }
        
        wait(0.1f);
    }
}
