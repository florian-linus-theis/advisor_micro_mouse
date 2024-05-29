//#include <iostream>
#pragma once
#include "Arduino.h"
#include "Pin_Setup.h"
#include "Systick.h"

HardwareTimer timer14(TIM14);
HardwareTimer timer3(TIM3);
HardwareTimer timer4(TIM4);
HardwareTimer timer2(TIM2);
HardwareTimer timer5(TIM5);
HardwareTimer timer6(TIM6);
HardwareTimer timer7(TIM7);
HardwareTimer timer10(TIM10);
HardwareTimer timer1(TIM1);

#include "Sensors.h"


void Systick_Setup();
void Timer3_Setup();
void Timer4_Setup();
void Timer2_Setup();
void Timer5_Setup();
void Timer6_Setup();
void Timer7_Setup();
void Timer10_Setup();
void Timer1_Setup(); 


void Timer_Setup() {
//Systick Timer14
    Systick_Setup();

//Motor Timers
    Timer3_Setup(); //PWM Timer Left        //TO-DO
    Timer4_Setup(); //PWM Timer Right       //TO-DO
    Timer2_Setup(); //Encoder Timer Left    //TO-DO / Overhaul
    Timer5_Setup(); //Encoder Timer Right   //TO-DO / Overhaul

//Infrared Timers
    Timer6_Setup(); //Main Interrupt Timer
    Timer7_Setup(); //Mid Sensor Interrupt Timer

//Servo1 PWM Timer
    Timer10_Setup();                        //TO-DO

//Buzzer PWM Timer
    Timer1_Setup();                         //TO-DO
}



void Systick_Setup(void) {
    timer14.setPrescaleFactor(122000);          // Set prescaler to 122000
    timer14.setOverflow(65535);                 // Set overflow to 65535 = 2 ms intervals
    timer14.attachInterrupt(Systick_Interrupt);  
    timer14.refresh();
    timer14.resume();
}


void Timer3_Setup() {   //Motor PWM Left
    timer3.setMode(1, TIMER_OUTPUT_COMPARE_PWM1, MOTOR_LEFT_PWM_1);
    timer3.setMode(2, TIMER_OUTPUT_COMPARE_PWM1, MOTOR_LEFT_PWM_2);
    timer3.setPrescaleFactor(4);
    timer3.setOverflow(1000);
    timer3.setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    timer3.setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
    timer3.resume();
}
void Timer4_Setup() {   //Motor PWM Right
    timer4.setMode(1, TIMER_OUTPUT_COMPARE_PWM1, MOTOR_RIGHT_PWM_1);
    timer4.setMode(2, TIMER_OUTPUT_COMPARE_PWM1, MOTOR_RIGHT_PWM_2);
    timer4.setPrescaleFactor(4);
    timer4.setOverflow(1000);
    timer4.setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    timer4.setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
    timer4.resume();
}


void Timer2_Setup(){    //Motor Encoder Left
    //configuring Timer in Encoder Mode sadly not possible with HardwareTimer Lib...
    //look at this shit :/
    //brother ehhhwww
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
    timer6.setPrescaleFactor(50);               // Set prescaler to 50
    timer6.setOverflow(16);                     // Set overflow to 16 = 50us intervals
    timer6.attachInterrupt(Timer6_Interrupt);
    timer6.refresh();
    timer6.pause();
}

void Timer7_Setup(void) {   //Mid Infrared Sensor Interrupt Timer
    // Configure timer
    timer7.setPrescaleFactor(400);              // Set prescaler to 50
    timer7.setOverflow(60);                     // Set overflow to 16 = 50us intervals
    timer7.attachInterrupt(Timer7_Interrupt);
    timer7.refresh();
    timer7.pause();
}


void Timer10_Setup() {      //Servo1 PWM TImer

}


void Timer1_Setup() {       //Buzzer PWM TImer

}