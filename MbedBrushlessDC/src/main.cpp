#include <mbed.h>
#include <iostream>
#include "AS5601.h"

//#define PWM_PERIOD 0.00001f
//#define TIM_USR TIM7
//#define TIM_USR_IRQ TIM7_IRQn
#define TEMPO_ATT1 10

Serial pc(USBTX, USBRX, 9600);
AS5601 encoder(PB_9,PB_8); // PB_9=SDA, PB_8=SCL

float angle = 0;
int step_number = 0;
float position;
AnalogIn ain(PB_1);
AnalogIn phase_B_curr(PC_1);

float pwm_positive=0.5f;
float gnd_negative=0.0f;

//DigitalOut led1(LED1);
PwmOut uh_1(PA_8);
PwmOut vh_2(PA_9);
PwmOut wh_3(PA_10);

DigitalOut en_1(PC_10);
DigitalOut en_2(PC_11);
DigitalOut en_3(PC_12);


// This function converts hall sensor's angle into 6 electrical positions of a BLDC motor
void stepRead()
{
  // Check in which of 6 position the motor is
  if(position<=6.0f || position>47.0f)
  {
    step_number=5;
  }
  else if(position>6.0f && position<=13.0f)
  {
    step_number=4;
  }
  else if(position>13.0f && position<=20.98f)
  {
    step_number=3;
  }
  else if(position>20.98f && position<=30.0f)
  {
    step_number=2;
  }
  else if(position>30.0f && position<=43.0f)
  {
    step_number=1;
  }
  else if(position>43.0f && position<=48.0f)
  {
    step_number=0;
  }
}



int main() {
    float angle;
    double seno;
    while(1) {
        angle = encoder.get_angle_degrees();
        position = fmod(angle,51.43f); //360/7=51.43, where 7 is number of magnets divided by 2
        //stepRead();
        step_number++;
        step_number%=6;
        switch(step_number) {
            case 0:
                wh_3.write(gnd_negative);
                en_1=1;
                en_2=0;
                en_3=1;
                for(double i=0.0;i<1.57;i+=0.01){
                    seno = sin(i);
                    uh_1.write(seno);
                    vh_2.write(sqrt(1-(pow(seno,2))));
                    wait_us(TEMPO_ATT1);
                }
                break;
            case 1:
                //uh_1.write(0.0f);
                //vh_2.write(pwm_positive);
                wh_3.write(gnd_negative);
                en_1=0;
                en_2=1;
                en_3=1;
                for(double i=0.0;i<1.57;i+=0.01){
                    seno = sin(i);
                    vh_2.write(sin(i));
                    uh_1.write(sqrt(1-(pow(seno,2))));
                    wait_us(TEMPO_ATT1);
                }
                break;
            case 2:
                uh_1.write(gnd_negative);
                //vh_2.write(pwm_positive);
                //wh_3.write(0.0f);
                en_1=1;
                en_2=1;
                en_3=0;
               
                for(double i=0.0;i<1.57;i+=0.01){
                    seno = sin(i);
                    vh_2.write(sin(i));
                    wh_3.write(sqrt(1-(pow(seno,2))));
                    wait_us(TEMPO_ATT1);
                }
                break;
            case 3:
                uh_1.write(gnd_negative);
                //vh_2.write(0.0f);
                //wh_3.write(pwm_positive);
                en_1=1;
                en_2=0;
                en_3=1;
               
                for(double i=0.0;i<1.57;i+=0.01){
                    seno = sin(i);
                    wh_3.write(sin(i));
                    vh_2.write(sqrt(1-(pow(seno,2))));
                    wait_us(TEMPO_ATT1);
                }
                break;
            case 4:
                //uh_1.write(0.0f);
                vh_2.write(gnd_negative);
                //wh_3.write(pwm_positive);
                en_1=0;
                en_2=1;
                en_3=1;
               
                for(double i=0.0;i<1.57;i+=0.01){
                    seno = sin(i);
                    wh_3.write(sin(i));
                    uh_1.write(sqrt(1-(pow(seno,2))));
                    wait_us(TEMPO_ATT1);
                }
                break;
            case 5:
                //uh_1.write(pwm_positive);
                vh_2.write(gnd_negative);
                //wh_3.write(0.0f);
                en_1=1;
                en_2=1;
                en_3=0;
               
                for(double i=0.0;i<1.57;i+=0.01){
                    seno = sin(i);
                    uh_1.write(sin(i));
                    wh_3.write(sqrt(1-(pow(seno,2))));
                    wait_us(TEMPO_ATT1);
                }
                break;
        }
        

        pc.printf("%d) angle: %f - posit: %f\n",step_number,angle,position);
        
        
        
        wait_us(1);
    }
}