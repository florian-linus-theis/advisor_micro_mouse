#pragma once
#include <Setup\Setup.h>

// Für Servo 
#define TIMER_FREQUENCY 142000000 // 168 MHz

// Servo pulse widths (in microseconds)
const uint16_t PULSE_WIDTH_0 = 500; // 0.5 ms
const uint16_t PULSE_WIDTH_90 = 1500; // 1.5 ms
const uint16_t PULSE_WIDTH_180 = 1950; // ~2 ms (not exactly 180 degrees (rather 150) but angle we need for MM)

// Timer calculations
const uint16_t PRESCALER = 1680 - 1; // Prescaler to get 100 kHz timer frequency
const uint16_t PERIOD = 2000 - 1; // Period to get 50 Hz PWM frequency




void writeServo(int angle) {
    // Map the angle to the pulse width
    int decimal = 0; 
    switch (angle) {
        case 0:
            decimal = 1638;
            break;
        case 90:
            decimal = 4915;
            break;
        case 180:
            decimal = 8192;
            break;
    }

    // Update the PWM duty cycle
    timer10->setCaptureCompare(1, decimal, TICK_COMPARE_FORMAT);
    timer10->refresh();
    timer10->resume();
}



// Function to move the ball grabber forward (in front of robot)
void move_ballgrabber_forward() {
    timer4->setCaptureCompare(3, PULSE_WIDTH_0, MICROSEC_COMPARE_FORMAT); // 0 degrees
    timer4->refresh();
    timer4->resume();
}

// Function to move the ball grabber backward (above robot)
void move_ballgrabber_backward() {
    timer4->setCaptureCompare(3, PULSE_WIDTH_180, MICROSEC_COMPARE_FORMAT); // 180 degrees
    timer4->refresh();
}


void handle_ballgrabber(){
    move_ballgrabber_forward();
    ble->print("ballgrabber forward");
    delay(1000);
    move_ballgrabber_backward();
}