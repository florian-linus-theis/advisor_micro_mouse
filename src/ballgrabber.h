#pragma once
#include <Setup\Setup.h>

// Servo pulse widths (in microseconds)
const uint16_t PULSE_WIDTH_0 = 500; // 0.5 ms
const uint16_t PULSE_WIDTH_180 = 2500; // 0.5 ms
const uint16_t DEGREE_FRONT = 0;
const uint16_t DEGREE_BACK = 131;



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
    digitalWrite(SERVO_ENABLE, HIGH);
    delay(10); // TODO: improve here, maybe cancel this out completely
    writeServo(DEGREE_FRONT);
    delay(1000);
    writeServo(DEGREE_BACK);
    delay(500); // TODO: improve here, maybe cancel this out completely
    digitalWrite(SERVO_ENABLE, LOW);
}