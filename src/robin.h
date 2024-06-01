#pragma once
#include <Arduino.h>
#include <wiring.h>
#include "HardwareTimer.h"
#include "stm32f4xx_hal.h"
#include "Setup\Setup.h"
#include <display.h>
#include "Sensors.h"

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

void Mid_Sensor_Interrupt(void);
void Mid_Sensor_Setup(void);
void Distanz_Mid_Sensor(void);

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


void robin_test() {
    digitalWrite(MOTOR_ENABLE, LOW); // turn motors on (if not already on) 
    for(int i = 0; i < 2; i++){
        // Wait for the encoder to be turned
        digitalWrite(MOTOR_ENABLE, LOW);
        ForwardBoth(10);
        delay(1000);
        if (encoderTurned) {
            encoderTurned = false;
            break;
        }
        BackwardBoth(10);
        delay(1000);
        if (encoderTurned) {
            encoderTurned = false;
            break;
        }
    }
    // Stop the motors
    digitalWrite(MOTOR_ENABLE, HIGH);
}



void test_encoders(){
    display->clearDisplay();
    display->println("Testing Encoders2");
    for (int i = 0; i < 10; i++) {
        // display->clearDisplay();
        display->clearDisplay();
        int encoder_right = TIM5->CNT;
        int encoder_left = TIM2->CNT;
        ble->print("Encoder Right: ");
        ble->println(encoder_right);
        ble->print("Encoder Left: ");
        ble->println(encoder_left);
        display->print("Encoder R: ");
        display->println(encoder_right);
        display->print("Encoder L: ");
        display->println(encoder_left); 
        delay(1000);
    }
}


