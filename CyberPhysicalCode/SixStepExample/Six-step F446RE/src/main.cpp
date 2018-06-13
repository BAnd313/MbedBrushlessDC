/*
 
    Nucleo f446re timers:
        General purpose:
            TIM2/3/4/5              -> 32 bit
            TIM9/10/11/12/13/14     -> 16 bit
        
        Basic:
            TIM6/7                  -> 16bit
    
 -----------------------------------------------------------------------
 
    Nucleo f302xB/C/D/E o f302x6/8:
        General purpose: 
            TIM1/3/4/15/16/17 -> 16 bit
            TIM2              -> 32 bit
        
        Basic:
            TIM6              -> 16 bit
*/



#include "mbed.h"
#include "math.h"
#include "AS5601.h"

// Constant to convert potentiometer value into current value
#define C 2
// Max integral error
#define IERR_MAX 4096
// Min integral error
#define IERR_MIN -4096
// PWM frequency 100kHz
#define PWM_PERIOD 0.00001f
// Internal Timer 7
#define TIM_USR      TIM3
#define TIM_USR_IRQ  TIM3_IRQn


// Serial communication
Serial pc(USBTX, USBRX, 9600); // tx, rx, speed

// AS5601
AS5601 encoder(PB_9,PB_8);

// variable to keep angle value from hall sensor
float angle = 0;
// Analog input to read potentiometer
AnalogIn  ain(PB_1);
// Analog input to read motor current shunt resistor
AnalogIn  phase_B_curr(PC_1);
// Accumulator for analog input potentiometer
int sum = 0;
// Error between motor current and potentiometer
int err = 0;
// Error accumulator
int ierr = 0;
// Time counter for slower tasks
volatile int cnt_time = 0;
// Current filter counter
char cnt_current = 0;
// Current accumulator for current filter
int sum_current = 0;
// Analog potentiometer filter counter
char cnt = 0;
// Flag for interrupt timer 7
volatile char flag_time = 0;
// Actual motor step
volatile char step_number = 0;
// Old motor step
char step_number_old = 0;
// Angle value scaled by electrical poles
volatile float position = 0;
// Flag motor started ON=1 OFF=0
char motor_on = 0;
// Desired current value, read form potentiometer
int current_val = 0;
// Potentiometer value
int pot = 0;
// Flag button pressed PRESSED=1 NOT PRESSED=0
char pressed = 0;
// Current motor value
int icurrent = 0;
// PWM duty cycle for motor phase
float duty_cycle = 0.0f;
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
// BUTTON
DigitalIn button(USER_BUTTON);

float K = 0.85f;

// Timer handler
TIM_HandleTypeDef mTimUserHandle;

// This function handle TIM3 interrupt, it simply set flag_time to 1 to save execution time
extern "C"

void M_TIM_USR_Handler(void) {
  if (__HAL_TIM_GET_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE) == SET) {
      __HAL_TIM_CLEAR_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE);
      flag_time = 1;
      led1= !led1;
  }
}

// This function calculate motor duty_cycle using a PI
// The PI works comparing potentiometer value and motor current value
void pi()
{
  // Read desired current value from potentiometer
  current_val = pot >> C;
  // Calculate error between desired current value and actual current value
  err = (current_val - icurrent);
  
  // Accumulate current error for integral PI part
  ierr += err;
  // Prevent saturation of IERR
  if(ierr > IERR_MAX)
    ierr = IERR_MAX;
  if(ierr < IERR_MIN)
    ierr = IERR_MIN;

  // PI function to obtain duty_cycle
  duty_cycle = (float) ((float)(err*K)/1000.0f);
  // Prevent duty_cycle acceptable values
  //duty_cycle -= 0.2f; // removing 0.2f because of duty_cycle is between ~0.2f and ~1.2f
  if(duty_cycle < 0.0f)
    duty_cycle = 0.0f;
  else if(duty_cycle > 1.0f)
    duty_cycle = 1.0f;
  
  pwm_positive = duty_cycle;
  gnd_negative = 0.0f;

/*
  if(duty_cycle <= 0.45f) {
    duty_cycle = 0.45f-duty_cycle;
    duty_cycle /= 0.45f;
    pwm_positive = duty_cycle;
    gnd_negative = 0.0f;
  } else if(duty_cycle >= 0.55f) {
    duty_cycle -= 0.55f;
    duty_cycle /= 0.45f;
    pwm_positive = 0.0f;
    gnd_negative = duty_cycle;
  } else {
    pwm_positive = 0.0f;
    gnd_negative = 0.0f;
  }
  */
}

// This function reads potentiometer value filtering with mean between 2 reads
void analogRead()
{
  // Accumulate reads
  sum += ain.read_u16()>>4;
  
  // Count reads
  cnt++;
  if(cnt >= 2)
  {
    cnt = 0;
	  
    // pot is the definitive potentiometer value
    pot = sum>>1;
	  
    // Reset accumulator
    sum = 0;
  }
}

// This function converts hall sensor's angle into 6 electrical positions of a BLDC motor
void stepRead()
{
  // Check in which of 6 position the motor is
  if(position<=6.0f || position>50.5f)
  {
    step_number=3;
  }
  if(position>6.0f && position<=16.47f)
  {
    step_number=4;
  }
  if(position>16.47f && position<=24.98f)
  {
    step_number=5;
  }
  if(position>24.98f && position<=33.0f)
  {
    step_number=0;
  }
  if(position>33.0f && position<=41.0f)
  {
    step_number=1;
  }
  if(position>41.0f && position<50.5f)
  {
    step_number=2;
  }
}


// This function reads button state to START/STOP motor
void buttonRead()
{
  if(!button)
  {
	  // Set button state to pressed
    pressed = 1;
  }
  else
  {
    if(pressed)
    {
	    // Semaphore to change motor status only on time at each press
      pressed = 0;
	    
      // Reset integral error
      ierr=0;
	    
      // Change motor status RUNNING or STOPPED
      motor_on = !motor_on;
    }
  }
}

// This function applies the 6-step algorithm to run the motor
void step_forward()
{
  // Check if motor has to run
  if(motor_on==1)
  {
	  // Enable motor driver chip
    if(!en_chip)
    {
      en_chip = 1;
    }

	  // Read step
    stepRead();

	  // Switch through step values to activate correct phases
	  // For each step we have one phase with positive voltage PWM, one phase connected to ground and one phase disconnected
    switch(step_number)
    {
      case 0:
        uh_1.write(pwm_positive);		//PWM
        vh_2.write(0.0f);			//DISCONNECTED
        wh_3.write(gnd_negative);			//GND
        en_1 = 1;					//PWM
        en_2 = 0;					//DISCONNECTED
        en_3 = 1;					//GND
        break;

      case 1:
        uh_1.write(0.0f);			//DISCONNECTED
        vh_2.write(pwm_positive);		//PWM
        wh_3.write(gnd_negative);			//GND
        en_1 = 0;					//DISCONNECTED
        en_2 = 1;					//PWM
        en_3 = 1;					//GND
        break;

      case 2:
        uh_1.write(gnd_negative);			//GND
        vh_2.write(pwm_positive);		//PWM
        wh_3.write(0.0f);			//DISCONNECTED
        en_1 = 1;					//GND
        en_2 = 1;					//PWM
        en_3 = 0;					//DISCONNECTED
        break;

      case 3:
        uh_1.write(gnd_negative);			//GND
        vh_2.write(0.0f);			//DISCONNECTED
        wh_3.write(pwm_positive);		//PWM
        en_1 = 1;					//GND
        en_2 = 0;					//DISCONNECTED
        en_3 = 1;					//PWM
        break;

      case 4:
        uh_1.write(0.0f);			//DISCONNECTED
        vh_2.write(gnd_negative);			//GND
        wh_3.write(pwm_positive);		//PWM
        en_1 = 0;					//DISCONNECTED
        en_2 = 1;					//GND
        en_3 = 1;					//PWM
        break;

      case 5:
        uh_1.write(pwm_positive);		//PWM
        vh_2.write(gnd_negative);			//GND
        wh_3.write(0.0f);			//DISCONNECTED
        en_1 = 1;					//PWM
        en_2 = 1;					//GND
        en_3 = 0;					//DISCONNECTED
        break;

      default:
        break;
    }
  }
  // If motor is stopped it disables the motor driver chip
  else
  {
    en_chip = 0;
  }
}


// This is the main function
int main() {
  // Code to print float values on serial
  asm(".global _printf_float");

  pc.printf("Loading...\n");
  wait(1.0);
  en_chip = 1;    // Enable motor driver chip
  pc.printf("Activating the motor...\n");

  // Enable timer 3
    // equivalent of __GPIOA_CLK_ENABLE();
  __HAL_RCC_TIM3_CLK_ENABLE();

  // Set timer 3 values to work at 200us
  // instance indica l'indirizzo del registro base, ovvero, del timer che vogliamo utilizzare in base al processore che stiamo adoperando
  mTimUserHandle.Instance = TIM_USR;
    
  // Sotto struttura init
    /*
        Prescaler: valore per il quale viene diviso il tim clock.
        CounterMode: modalità di incremento del count specifica se contare per ogni fronte di salita o di discesa (?)
        Period: Specifica il valore del periodo che deve essere caricato dentro il registro auto-reload e viene caricato al prossimo evento di update.

        Freq = 1 kHz = 90000000 / 900 / (99+1)
     */
  mTimUserHandle.Init.Prescaler         = 900-1;//4799;
  mTimUserHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  mTimUserHandle.Init.Period            = 100-1;
  mTimUserHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    
    /*
        Inizializza il TIM_Base
     */
  HAL_TIM_Base_Init(&mTimUserHandle);
    /*
        Inizia il time_base e abilita l'interrupt (IT)
     */
  HAL_TIM_Base_Start_IT(&mTimUserHandle);
  
  // Enable timer 7 interrupt
  NVIC_SetVector(TIM_USR_IRQ, (uint32_t)M_TIM_USR_Handler);
  NVIC_EnableIRQ(TIM_USR_IRQ);

  // Set PWM period and initializes enables pins
  uh_1.period(PWM_PERIOD);
  vh_2.period(PWM_PERIOD);
  wh_3.period(PWM_PERIOD);
  en_1 = 0;
  en_2 = 0;
  en_3 = 0;

  pc.printf("Ready. Press the button to start.\n");
  motor_on=1;
  
  int cnt_print = 0;

  // Main loop
  while (true) {
	  // If timer 7 has triggered it enters in if statement
    if(flag_time >= 1)
    {
	    // Reset flag
      flag_time=0;
	    // Counter for slower tasks
      cnt_time++;
	    
      // Tasks slower than others
      if(cnt_time>32)
      {
		    // Calculate duty_cycle
        if(motor_on==1)
          pi();
        cnt_time=0;
		    
        // Read button state
        buttonRead();
		    
        // Read potentiometer
        analogRead();
      }

      if(cnt_print > 100){
        pc.printf("%f, %f\n", angle, phase_B_curr.read_u16() / 4096.0f * 1.4f);
        cnt_print = 0;
      }
      wait_ms(10); // TODO per cambiare la velocità del motore basta cambiare questo
      pc.printf("Position: %f\n", position);
      cnt_print++;

	    // Read hall sensor's angle
      angle = encoder.get_angle_degrees();
      
      // Read position from hall sensor's angle divided by electrical poles
      position = fmod(angle,51.43f); //360/7=51.43, where 7 is number of magnets divided by 2
      
      // Checks step value and eventually runs motor
      step_forward();
	  
	    // Accumulates current value 0.33*2.8*1.53*2
      sum_current += phase_B_curr.read_u16()>>4;
	    
      // Accumulates 16 values and then means them
      cnt_current++;
      if(cnt_current >= 16)
      {
        cnt_current = 0;
		    
        // Read motor current value filtered
        icurrent = sum_current>>4;
        sum_current = 0;
      }
    }
  }
}
