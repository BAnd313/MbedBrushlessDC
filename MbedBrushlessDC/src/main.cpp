#include "mbed.h"
#include "math.h"
#include "AS5601.h"

// PWM frequency 100kHz
#define PWM_PERIOD 0.00001f

// AS5601
AS5601 encoder(I2C_SDA, I2C_SCL);
Serial pc(USBTX, USBRX, 9600); // tx, rx, speed

// variable to keep angle value from hall sensor
float angle = 0;
// Actual motor step
volatile char step_number = 0;
// Angle value scaled by electrical poles
volatile float position = 0;
// PWM duty cycle for motor phase
float duty_cycle = 0.3f;
float pwm_positive = duty_cycle;
float gnd_negative = 0.0f;
// ON board LED
DigitalOut led1(LED1);
// Phase 1 PWM OUT
PwmOut uh_1(PA_8);
// Phase 2 PWM OUT
PwmOut vh_2(PA_9);
// Phase 3 PWM OUT
PwmOut wh_3(PA_10);
// Phase 1 ENABLE PIN
DigitalOut en_1(PC_10);
// Phase 2 ENABLE PIN
DigitalOut en_2(PC_11);
// Phase 3 ENABLE PIN
DigitalOut en_3(PC_12);
// MOTOR DRIVER CHIP ENABLE PIN
DigitalOut en_chip(PA_6);
// Potentiometer read
AnalogIn ain(PB_1);

void read_potentiometer()
{
  pwm_positive=ain.read();
}

// This function converts hall sensor's angle into 6 electrical positions of a BLDC motor
void stepRead()
{
  // Check in which of 6 position the motor is
  if (position <= 8.97f || position > 50.5f)
  {
    step_number = 5;
  }
  if (position > 8.35f && position <= 17.49f)
  {
    step_number = 4;
  }
  if (position > 16.87f && position <= 26.11f)
  {
    step_number = 3;
  }
  if (position > 25.49f && position <= 34.81f)
  {
    step_number = 2;
  }
  if (position > 33.84f && position <= 43.2f)
  {
    step_number = 1;
  }
  if ((position > 42.47f && position < 53.0f) || position <= 0.87f)
  {
    step_number = 0;
  }
}

// This function applies the 6-step algorithm to run the motor
void step_forward()
{
  // Enable motor driver chip
  if (!en_chip)
  {
    en_chip = 1;
  }

  // Read step
  stepRead();

  // Switch through step values to activate correct phases
  // For each step we have one phase with positive voltage PWM, one phase connected to ground and one phase disconnected
  switch (step_number)
  {
  case 0:
    uh_1.write(pwm_positive); //PWM
    vh_2.write(0.0f);         //DISCONNECTED
    wh_3.write(gnd_negative); //GND
    en_1 = 1;                 //PWM
    en_2 = 0;                 //DISCONNECTED
    en_3 = 1;                 //GND
    break;

  case 1:
    uh_1.write(0.0f);         //DISCONNECTED
    vh_2.write(pwm_positive); //PWM
    wh_3.write(gnd_negative); //GND
    en_1 = 0;                 //DISCONNECTED
    en_2 = 1;                 //PWM
    en_3 = 1;                 //GND
    break;

  case 2:
    uh_1.write(gnd_negative); //GND
    vh_2.write(pwm_positive); //PWM
    wh_3.write(0.0f);         //DISCONNECTED
    en_1 = 1;                 //GND
    en_2 = 1;                 //PWM
    en_3 = 0;                 //DISCONNECTED
    break;

  case 3:
    uh_1.write(gnd_negative); //GND
    vh_2.write(0.0f);         //DISCONNECTED
    wh_3.write(pwm_positive); //PWM
    en_1 = 1;                 //GND
    en_2 = 0;                 //DISCONNECTED
    en_3 = 1;                 //PWM
    break;

  case 4:
    uh_1.write(0.0f);         //DISCONNECTED
    vh_2.write(gnd_negative); //GND
    wh_3.write(pwm_positive); //PWM
    en_1 = 0;                 //DISCONNECTED
    en_2 = 1;                 //GND
    en_3 = 1;                 //PWM
    break;

  case 5:
    uh_1.write(pwm_positive); //PWM
    vh_2.write(gnd_negative); //GND
    wh_3.write(0.0f);         //DISCONNECTED
    en_1 = 1;                 //PWM
    en_2 = 1;                 //GND
    en_3 = 0;                 //DISCONNECTED
    break;

  default:
    break;
  }
}

// This is the main function
int main()
{
  en_chip = 1; // Enable motor driver chip

  // Set PWM period and initializes enables pins
  uh_1.period(PWM_PERIOD);
  vh_2.period(PWM_PERIOD);
  wh_3.period(PWM_PERIOD);
  en_1 = 0;
  en_2 = 0;
  en_3 = 0;

  // Main loop
  while (true)
  {
    // Read hall sensor's angle
    angle = encoder.get_angle_degrees();

    // Read position from hall sensor's angle divided by electrical poles
    position = fmod(angle, 51.43f); //360/7, where 7 is number of magnets divided by 2

    // Read the potentiometer value and set the duty cycle
    read_potentiometer();

    // Checks step value and eventually runs motor
    step_forward();
  }
}
