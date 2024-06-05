#pragma once
#include "Setup\Setup.h"
#include <display.h>

extern bool encoderTurned;

void BackwardLeft(int dutyCycle) {
    timer3->setCount(0);
    timer3->resume();
    timer3->setCaptureCompare(1, dutyCycle, TICK_COMPARE_FORMAT);
    timer3->setCaptureCompare(2, 0, TICK_COMPARE_FORMAT);
    timer3->refresh();
}

void ForwardLeft(int dutyCycle) {
    int correction = round(dutyCycle * 0.037);
    timer3->setCount(0);
    timer3->resume();
    timer3->setCaptureCompare(1, 0, TICK_COMPARE_FORMAT);
    timer3->setCaptureCompare(2, dutyCycle + correction, TICK_COMPARE_FORMAT);
    timer3->refresh();
}

void ForwardRight(int dutyCycle) {
    timer4->setCount(0);
    timer4->resume();
    timer4->setCaptureCompare(1, dutyCycle, TICK_COMPARE_FORMAT);
    timer4->setCaptureCompare(2, 0, TICK_COMPARE_FORMAT);
    timer4->refresh();
}

void BackwardRight(int dutyCycle) {
    timer4->setCount(0);
    timer4->resume();
    timer4->setCaptureCompare(1, 0, TICK_COMPARE_FORMAT);
    timer4->setCaptureCompare(2, dutyCycle, TICK_COMPARE_FORMAT);
    timer4->refresh();
}

void ForwardBoth(int dutyCycle) {
    // double factor = dutyCycle / 100.0;
    // int correctionL = static_cast<int>(round(dutyCycle + (PID_values[0] * factor)));
    // int correctionR = static_cast<int>(round(dutyCycle + (PID_values[1] * factor)));
    if(dutyCycle + PID_values[0] > 0){
        ForwardLeft(dutyCycle + PID_values[0]);
    }
    else if(dutyCycle + PID_values[0] <= 0){
        ForwardLeft(0);
    }

    if(dutyCycle + PID_values[1] > 0){
        ForwardRight(dutyCycle + PID_values[1]);
    }
    else if(dutyCycle + PID_values[1] <= 0){
        ForwardRight(0);
    }
    current_duty_cycle = dutyCycle;
}

void BackwardBoth(int dutyCycle) {
    BackwardLeft(dutyCycle);
    BackwardRight(dutyCycle);
    current_duty_cycle = dutyCycle;
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
        // BackwardBoth(10);
        // delay(1000);
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


