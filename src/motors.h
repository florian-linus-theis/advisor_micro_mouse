#pragma once
#include "Setup/Setup.h"

std::vector<int> determine_correction_needed(){
    std::vector<int> chosen_correction = {0,0};
    if(PID_ENABLED == false){
        chosen_correction = {0,0};
    }
    else if(abs(remapped_error[CURRENT_CASE_PID]) > 2){
        chosen_correction = PID_values;
        //ble->println("IR");
    }
    else{
        distance_traveled_L_PID = 0;
        distance_traveled_R_PID = 0;
        chosen_correction = PID_values_encoder;
        //ble->println("ENC");
    }
    return chosen_correction;
}

void BackwardLeft(int dutyCycle) {
    timer3->setCount(0);
    timer3->resume();
    timer3->setCaptureCompare(1, dutyCycle, TICK_COMPARE_FORMAT);
    timer3->setCaptureCompare(2, 0, TICK_COMPARE_FORMAT);
    timer3->refresh();
}

void ForwardLeft(int dutyCycle) {
    // timer3->setCount(0);
    // timer3->resume();
    timer3->setCaptureCompare(1, 0, TICK_COMPARE_FORMAT);
    timer3->setCaptureCompare(2, dutyCycle, TICK_COMPARE_FORMAT);
    // timer3->refresh();
}

void ForwardRight(int dutyCycle) {
    // timer4->setCount(0);
    // timer4->resume();
    timer4->setCaptureCompare(1, dutyCycle, TICK_COMPARE_FORMAT);
    timer4->setCaptureCompare(2, 0, TICK_COMPARE_FORMAT);
    // timer4->refresh();
}

void BackwardRight(int dutyCycle) {
    timer4->setCount(0);
    timer4->resume();
    timer4->setCaptureCompare(1, 0, TICK_COMPARE_FORMAT);
    timer4->setCaptureCompare(2, dutyCycle, TICK_COMPARE_FORMAT);
    timer4->refresh();
}

void ForwardBoth(int dutyCycle){
    if(dutyCycle + PID_values[0] > 0){
        ForwardLeft(dutyCycle + PID_values[0]);
    }
    else if(dutyCycle + PID_values[0] <= 0){
        ForwardLeft(0);
    }

    if(dutyCycle +  PID_values[1] > 0){
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


