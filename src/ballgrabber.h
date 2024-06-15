#pragma once
#include <Setup\Setup.h>

// Servo pulse widths (in microseconds)
const uint16_t PULSE_WIDTH_0 = 500; // 0.5 ms
const uint16_t PULSE_WIDTH_180 = 2500; // 0.5 ms
const uint16_t DEGREE_FRONT = 110;
const uint16_t DEGREE_BACK = 0;
const uint16_t DEGREE_GROUND = 140;



void writeServo(int angle) {
    // Map the angle to the pulse width
    int decimal = constrain(map(angle, 0, 180, PULSE_WIDTH_0, PULSE_WIDTH_180), PULSE_WIDTH_0, PULSE_WIDTH_180);
    timer10->setCaptureCompare(1, decimal, TICK_COMPARE_FORMAT);
    timer10->refresh();
    timer10->resume();
}


// Function to move the ball grabber forward (in front of robot)
void move_ballgrabber_forward() {
    writeServo(DEGREE_FRONT);
}


// Function to move the ball grabber backward (above robot)
void move_ballgrabber_backward() {
    writeServo(DEGREE_BACK);
}


//handling ballgrabber action including reset of motor timer and servo timer
void handle_ballgrabber(){
    writeServo(DEGREE_BACK);
    digitalWrite(SERVO_ENABLE, HIGH);
    delay(1000);
    writeServo(DEGREE_FRONT);
    delay(500);
    digitalWrite(SERVO_ENABLE, LOW);
}