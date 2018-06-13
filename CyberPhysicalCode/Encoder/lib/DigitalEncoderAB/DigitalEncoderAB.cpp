#include "DigitalEncoderAB.h"

DigitalEncoderAB::DigitalEncoderAB(float cpr, float gearRatio) {
    this->cpr = cpr;
    setGearRatio(gearRatio);
    setAngleFormat(angleContinuous);
    
    EncoderInitialise_TIM2();
}

/**
 *  Reset the count of the angle
 */
void DigitalEncoderAB::reset() {
    TIM2->CNT=0;
}

/**
 *  Read the incremental integer angle
 *  @return Angle read
 */
int DigitalEncoderAB::getAngle() {
    return TIM2->CNT;
}

/**
 *  Get the max number of the integer angle
 *  @return Max angle
 */
int DigitalEncoderAB::getMaxAngle() {
    return cpr * getGearRatio();
}

/**
 *  Set the count per revolution of the encoder
 */
void DigitalEncoderAB::setCountPerRevolution(float cpr) {
    this->cpr = cpr;
}

/**
 *  Get the count per revolution of the encoder
 *  @return Count per revolution
 */
float DigitalEncoderAB::getCountPerRevolution() {
    return cpr;
}

/**
 *  Initialize TIM2 
 */
void DigitalEncoderAB::EncoderInitialise_TIM2() // 32 bit
{
    // configure GPIO PA0 & PA1 as inputs for Encoder
    RCC->AHB1ENR |= 0x00000001;  // Enable clock for GPIOA

    GPIOA->MODER   |= GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 ;           //PA0 & PA1 as Alternate Function   /*!< GPIO port mode register,               Address offset: 0x00      */
    GPIOA->OTYPER  |= GPIO_OTYPER_OT_0 | GPIO_OTYPER_OT_1 ;                 //PA0 & PA1 as Inputs               /*!< GPIO port output type register,        Address offset: 0x04      */
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1 ;     // Low speed                        /*!< GPIO port output speed register,       Address offset: 0x08      */
    GPIOA->PUPDR   |= GPIO_PUPDR_PUPDR0_1 | GPIO_PUPDR_PUPDR1_1 ;           // Pull Down                        /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    GPIOA->AFR[0]  |= 0x00000011 ;                                          //  AF01 for PA0 & PA1              /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
    GPIOA->AFR[1]  |= 0x00000000 ;

    //                                  /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
    // configure GPIO PA5 & PB3 as inputs for Encoder
    __GPIOA_CLK_ENABLE();
    //__GPIOB_CLK_ENABLE();
    // configure TIM2 as Encoder input
    RCC->APB1ENR |= 0x00000001;  // Enable clock for TIM2

    TIM2->CR1   = 0x0001;     // CEN(Counter ENable)='1'     < TIM control register 1
    TIM2->SMCR  = 0x0003;     // SMS='011' (Encoder mode 3)  < TIM slave mode control register
    TIM2->CCMR1 = 0xF1F1;     // CC1S='01' CC2S='01'         < TIM capture/compare mode register 1
    TIM2->CCMR2 = 0x0000;     //                             < TIM capture/compare mode register 2
    TIM2->CCER  = 0x0011;     // CC1P CC2P                   < TIM capture/compare enable register
    TIM2->PSC   = 0x0000;     // Prescaler = (0+1)           < TIM prescaler
    TIM2->ARR   = 0xffffffff; // reload at 0xfffffff         < TIM auto-reload register

    TIM2->CNT = 0x0000;  //reset the counter before we use it
}
