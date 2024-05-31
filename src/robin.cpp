
#include "HardwareTimer.h"
#include "stm32f4xx_hal.h"
#include <Setup.h>

extern bool encoderTurned;

// bool Error_Flag;
// int Flag_Mid;  
// int interrupt_counter;
// int Messung_Mid;
// uint32_t Distance_Sensor_Mid_MM;

// int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS};
// int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS};

// std::vector<uint32_t> Distanz_Sensoren;

// HardwareTimer timer3(TIM3);
// HardwareTimer timer4(TIM4);
HardwareTimer timer8(TIM8);
HardwareTimer timer9(TIM9);

HardwareSerial Serial1(BLUETOOTH_RX, BLUETOOTH_TX);

void Distanz_Messung_Hell(void);
void Sensor_Interrupt(void);
void Sensor_Sync_Setup(void);
void printDistanzSensoren(void);

void Mid_Sensor_Interrupt(void);
void Mid_Sensor_Setup(void);
void Distanz_Mid_Sensor(void);
void robin_test(void);

// void Timer3_Setup(void);
// void Timer4_Setup(void);

void BackwardLeft(int dutyCycle) {
    timer3->setCount(0);
    timer3->resume();
    timer3->setCaptureCompare(1, dutyCycle, PERCENT_COMPARE_FORMAT);
    timer3->setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
    timer3->refresh();
}

void ForwardLeft(int dutyCycle) {
    timer3->setCount(0);
    timer3->resume();
    timer3->setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    timer3->setCaptureCompare(2, dutyCycle, PERCENT_COMPARE_FORMAT);
    timer3->refresh();
}

void ForwardRight(int dutyCycle) {
    timer4->setCount(0);
    timer4->resume();
    timer4->setCaptureCompare(1, dutyCycle, PERCENT_COMPARE_FORMAT);
    timer4->setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
    timer4->refresh();
}

void BackwardRight(int dutyCycle) {
    timer4->setCount(0);
    timer4->resume();
    timer4->setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    timer4->setCaptureCompare(2, dutyCycle, PERCENT_COMPARE_FORMAT);
    timer4->refresh();
}

void ForwardBoth(int dutyCycle) {
    ForwardLeft(dutyCycle);
    ForwardRight(dutyCycle);
}

void BackwardBoth(int dutyCycle) {
    BackwardLeft(dutyCycle);
    BackwardRight(dutyCycle);
}


void Timer2_Setup(void){                //einziges funktionierendes Encoder Setup :)))
  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;
	GPIOA->MODER|=(1<<1)|(1<<3);
	GPIOA->AFR[0]|=(1<<0)|(1<<4);
	TIM2->ARR = 4294967295;
	TIM2->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0 ); 
	TIM2->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);  
	TIM2->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;   
	TIM2->CR1 |= TIM_CR1_CEN;
}


void Timer3_Setup() {   // Motor PWM Left
    timer3->setMode(1, TIMER_OUTPUT_COMPARE_PWM2, MOTOR_LEFT_PWM_1);
    timer3->setMode(2, TIMER_OUTPUT_COMPARE_PWM2, MOTOR_LEFT_PWM_2);
    timer3->setPrescaleFactor(8);
    timer3->setOverflow(1000);
    timer3->setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    timer3->setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
    timer3->refresh();
    timer3->resume();
}

void Timer4_Setup() {   // Motor PWM Right
   timer4->setMode(2, TIMER_OUTPUT_COMPARE_PWM2, MOTOR_RIGHT_PWM_1);
   timer4->setMode(1, TIMER_OUTPUT_COMPARE_PWM2, MOTOR_RIGHT_PWM_2);
   timer4->setPrescaleFactor(8);
   timer4->setOverflow(1000);
   timer4->setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
   timer4->setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
   timer4->refresh();
   timer4->resume();
}

void Timer5_Setup(void) {   //Encoder Setup links
    // Enable GPIOA, GPIOB, and TIM5 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    // Configure PA15 and PB3 as alternate function mode (AF)
    GPIOA->MODER |= (1 << (2*15)); // PA15
    GPIOB->MODER |= (1 << (2*3));  // PB3
    GPIOA->AFR[1] |= (1 << (4 * (15 - 8))); // PA15
    GPIOB->AFR[0] |= (1 << (4 * 3));        // PB3

    // Configure TIM5 settings
    TIM5->ARR = 4294967295;
    TIM5->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0); 
    TIM5->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);  
    TIM5->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;   
    TIM5->CR1 |= TIM_CR1_CEN;
}

void robin_test() {
    while(1) {
        // Wait for the encoder to be turned
        digitalWrite(MOTOR_ENABLE, LOW);
        ForwardBoth(10);
        delay(3000);
        if (encoderTurned) {
            encoderTurned = false;
            break;
        }
        BackwardBoth(10);
        delay(3000);
        if (encoderTurned) {
            encoderTurned = false;
            break;
        }
    }
    // Stop the motors
    digitalWrite(MOTOR_ENABLE, HIGH);
        // Other sensor or distance measuring code
}

int read_encoder_right_ticks(void){
    return TIM5->CNT;
}
int read_encoder_left_ticks(void){
    return TIM2->CNT;
}
