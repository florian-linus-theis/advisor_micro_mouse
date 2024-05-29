#pragma once
#include <Arduino.h>
#include <wiring.h>
#include "HardwareTimer.h"
#include "stm32f4xx_hal.h"
#include <Setup.h>


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

// void Timer3_Setup(void);
// void Timer4_Setup(void);

// Motor functions
void ForwardLeft(int dutyCycle) {
    timer3->setCount(0);
    timer3->resume();
    timer3->setCaptureCompare(1, dutyCycle, PERCENT_COMPARE_FORMAT);
    timer3->setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
    timer3->refresh();
}

void BackwardLeft(int dutyCycle) {
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

void robin_test() {
        digitalWrite(MOTOR_ENABLE, LOW);
        ForwardBoth(30);
        // Other sensor or distance measuring code
}


// // SENSORS --------------------------------------------------------------------------------------------------------

// void Sensor_Sync_Setup(void) {
//     // Configure timer
//     timer9.setPrescaleFactor(50); // Set prescaler to 50
//     timer9.setOverflow(33);       // Set overflow to 16 = 50us intervals
//     timer9.attachInterrupt(Sensor_Interrupt);
//     timer9.refresh();
//     timer9.pause();
// }

// void Sensor_Interrupt(void) {
//     // Read the current sensor value
//     Distanz_Sensoren.push_back(analogRead(Channel_Sensoren[interrupt_counter]));
//     // Turn off the current emitter
//     digitalWrite(Channel_Emitter[interrupt_counter], LOW);

//     // Move to the next sensor
//     interrupt_counter++;

//     if (interrupt_counter < 6) {
//         // Turn on the next emitter
//         digitalWrite(Channel_Emitter[interrupt_counter], HIGH);
//     } if (interrupt_counter >= 6) {
//         // All emitters processed
//         timer9.pause();
//         Error_Flag = false;
//         digitalWrite(Channel_Emitter[interrupt_counter], LOW);
//     }
// }

// void Distanz_Messung_Hell(void) {
//     // Reset the interrupt counter
//     interrupt_counter = 0;
//     Distanz_Sensoren.clear(); // Clear previous measurements

//     // Turn on the first emitter
//     digitalWrite(Channel_Emitter[0], HIGH);

//     // Enable the interrupt to start the measurement process
//     timer9.setCount(0);
//     timer9.refresh();
//     timer9.resume();
//     // NVIC_EnableIRQ(TIM9_IRQn);

//     // Wait for the process to complete
//     while (interrupt_counter < 6) {
//         // Wait in a non-blocking way (e.g., other code can run here)
//     }

//     // Disable the interrupt as the process is complete
//     // NVIC_DisableIRQ(TIM9_IRQn);
// }

// void printDistanzSensoren(void) {
//     Serial1.println("Distanz_Sensoren Messwerte:");
//     for (int i = 0; i < 6; i++) {
//         Serial1.print("Sensor ");
//         Serial1.print(i);
//         Serial1.print(": ");
//         Serial1.println(Distanz_Sensoren[i]);
//     }
    
//     Serial1.print("Distanz_Sensor Mitte:");
//     Serial1.println(Distance_Sensor_Mid_MM);
//     Serial1.print("Encoder L:");

//     uint32_t encoder_value = TIM2->CNT;
//     // int32_t encoder_value = timer2.getCount();
//     // int32_t encoder_value = MyTim->getCount();
//     // int32_t encoder_value = __HAL_TIM_GET_COUNTER(&htim2);
//     Serial1.println(encoder_value);
// }

// // Mid Sensor Implementation
// void Mid_Sensor_Setup(void) {
//     // Configure timer
//     timer8.setPrescaleFactor(400); // Set prescaler to 50
//     timer8.setOverflow(60);        // Set overflow to 16 = 50us intervals
//     timer8.attachInterrupt(Mid_Sensor_Interrupt);
//     timer8.refresh();
//     timer8.pause();
// }

// void Distanz_Mid_Sensor(void){
//     Flag_Mid = 0;
//     digitalWrite(IR_EMITTER_MID, HIGH);
//     timer8.setCount(0);
//     timer8.refresh();
//     timer8.resume();
    
//     while(Flag_Mid < 1){
//         // Wait for flag to be set in interrupt
//     }
// }

// void Mid_Sensor_Interrupt(void){
//     Distance_Sensor_Mid_MM = analogRead(IR_SENSOR_MID);
//     digitalWrite(IR_EMITTER_MID, LOW);
//     Flag_Mid++;
//     timer8.pause();
// }
