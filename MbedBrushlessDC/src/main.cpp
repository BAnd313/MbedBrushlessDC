#include <mbed.h>
#include <iostream>
#include "AS5601.h"

//#define PWM_PERIOD 0.00001f
//#define TIM_USR TIM7
//#define TIM_USR_IRQ TIM7_IRQn

Serial pc(USBTX, USBRX, 9600);
AS5601 encoder(PB_9,PB_8);

//float angle = 0;
//AnalogIn ain(PB_1);
//AnalogIn phase_B_curr(PC_1);

float pwm_positive=0.5f;
float gnd_negative=0.0f;

//DigitalOut led1(LED1);
PwmOut uh_1(PA_8);
PwmOut vh_2(PA_9);
PwmOut wh_3(PA_10);

DigitalOut en_1(PC_10);
DigitalOut en_2(PC_11);
DigitalOut en_3(PC_12);

int main() {
    int step_number=0;
    float angle;
    float position;
    while(1) {
        angle = encoder.get_angle_degrees();
        switch(step_number) {
            case 0:
                uh_1.write(pwm_positive);
                vh_2.write(0.0f);
                wh_3.write(gnd_negative);
                en_1=1;
                en_2=0;
                en_3=1;
                break;
            case 1:
                uh_1.write(0.0f);
                vh_2.write(pwm_positive);
                wh_3.write(gnd_negative);
                en_1=0;
                en_2=1;
                en_3=1;
                break;
            case 2:
                uh_1.write(gnd_negative);
                vh_2.write(pwm_positive);
                wh_3.write(0.0f);
                en_1=1;
                en_2=1;
                en_3=0;
                break;
            case 3:
                uh_1.write(gnd_negative);
                vh_2.write(0.0f);
                wh_3.write(pwm_positive);
                en_1=1;
                en_2=0;
                en_3=1;
                break;
            case 4:
                uh_1.write(0.0f);
                vh_2.write(gnd_negative);
                wh_3.write(pwm_positive);
                en_1=0;
                en_2=1;
                en_3=1;
                break;
            case 5:
                uh_1.write(pwm_positive);
                vh_2.write(gnd_negative);
                wh_3.write(0.0f);
                en_1=1;
                en_2=1;
                en_3=0;
                break;
        }
        position = fmod(angle,51.43f); //360/7=51.43, where 7 is number of magnets divided by 2

        pc.printf("%d,angle: %fposit: %f\n",step_number,angle,position);
        step_number++;
        step_number%=6;
        
        wait_ms(1000);
    }
}