#include "Setup.h"

//unused Timers 7,8,9,11,12,13
HardwareTimer *timer14 = new HardwareTimer(TIM14);
HardwareTimer *timer3 = new HardwareTimer(TIM3);
HardwareTimer *timer4 = new HardwareTimer(TIM4);
HardwareTimer *timer2 = new HardwareTimer(TIM2);
HardwareTimer *timer5 = new HardwareTimer(TIM5);
HardwareTimer *timer6 = new HardwareTimer(TIM6);
HardwareTimer *timer10 = new HardwareTimer(TIM10);
HardwareTimer *timer1 = new HardwareTimer(TIM1);



void Systick_Setup();
void Timer3_Setup();
void Timer4_Setup();
void Timer2_Setup();
void Timer5_Setup();
void Timer6_Setup();
void Timer10_Setup();
void Timer1_Setup(); 


void Timer_Setup() {    //Main Timer Setup - - - - - - - - - - - - - - - - - - - - - - -

   Systick_Setup();

//Motor Timers
    Timer3_Setup(); //PWM Timer Left        //TO-DO
    Timer4_Setup(); //PWM Timer Right       //TO-DO
    // Timer2_Setup(); //Encoder Timer Left    //TO-DO / Overhaul
    // Timer5_Setup(); //Encoder Timer Right   //TO-DO / Overhaul

//Infrared Timers
    Timer6_Setup(); //Main Interrupt Timer
    

//Servo1 PWM Timer
    //Timer10_Setup();                        //TO-DO

//Buzzer PWM Timer
    Timer1_Setup();                         //TO-DO
}




void Systick_Setup(void) {  //Systick Timer Setup
    timer14->setPrescaleFactor(10);          // Set prescaler to 122000
    timer14->setOverflow(32535);                 // Set overflow to 65535 = 2 ms intervals
    timer14->attachInterrupt(Systick_Interrupt);  
    timer14->refresh();
    timer14->pause();
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



void Timer5_Setup() {   //Motor Encoder Right
    //copy from Timer2
}



void Timer6_Setup(void) {   //Main Infrared Sensor Interrupt Timer
    timer6->setPrescaleFactor(50);               // Set prescaler to 50
    timer6->setOverflow(16);                     // Set overflow to 16 = 50us intervals
    timer6->attachInterrupt(Timer6_Interrupt);
    timer6->refresh();
    timer6->pause();
}


void Timer10_Setup() {      //Servo1 PWM TImer
    timer10->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, SERVO_PWM_1);
    timer10->setPrescaleFactor(26); // Set prescaler to 26
    timer10->setOverflow(64615); // Set overflow to 64615
    timer10->setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    timer10->refresh();
    timer10->resume();
}


void Timer1_Setup() {       //Buzzer PWM TImer

}

void setInterruptPriority(IRQn_Type irq, uint32_t preemptPriority, uint32_t subPriority) {
    HAL_NVIC_SetPriority(irq, preemptPriority, subPriority);
}

void set_Interrupt_Priority() {

    setInterruptPriority(EXTI15_10_IRQn, 0, 1);     //UI_Button und Encoder B
    setInterruptPriority(EXTI0_IRQn, 9, 1);         //Encoder A
    setInterruptPriority(EXTI2_IRQn, 0, 1);         //Motor Fehler

    timer2->setInterruptPriority(1,1);              //Motor Encoder links
    timer5->setInterruptPriority(1,2);              //Motor Encoder rechts
    timer6->setInterruptPriority(5,1);              //Main Infrared Sensor
    timer14->setInterruptPriority(7,1);             //Systick
}