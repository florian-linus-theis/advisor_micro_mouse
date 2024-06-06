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

// Sensor Calibrations 
std::vector<int> ballgrabber_calibration(7, 0);


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


//handling ballgrabber action including reset of motor timer and servo timer
void handle_ballgrabber(){
            digitalWrite(MOTOR_ENABLE, HIGH); // disable motor
            Timer4_Setup_Servo();
            digitalWrite(SERVO_ENABLE, HIGH);
            delay(1000); // TODO: improve here, maybe cancel this out completely
            timer4->setCaptureCompare(3, 500, MICROSEC_COMPARE_FORMAT); // 0 degrees
            delay(1000);
            timer4->setCaptureCompare(3, 1950, MICROSEC_COMPARE_FORMAT); // 180 degrees
            delay(1000); // TODO: improve here, maybe cancel this out completely
            digitalWrite(SERVO_ENABLE, LOW);
            Timer4_Setup_Motor();
            digitalWrite(MOTOR_ENABLE, LOW); // enable motor
}

// Full Function to grab the ball
void grab_ball(){
    CURRENT_CASE_PID = 2; // set PID case to 0
    reset_distance_traveled(); // reset distance to zero 
    reset_PID_values(); // reset PID values to zero
    move_forward_different(100, 0, 1.75); // move forward from very back to ball
    delay(500); // wait for wheels to really stop
    rotate_right(); // rotate right to face ball  // move forward to ball
    delay(500); // wait for wheels to really stop
    reset_PID_values(); // reset PID values to zero
    std::vector<int> original_calibration_values = NeutralSensorValues;
    NeutralSensorValues = ballgrabber_calibration;
    move_forward_different(100, 0, 1);  // move forward to ball
    delay(500); // wait for wheels to really stop
    CURRENT_CASE_PID = 4;
    pid_move_function(50);
    delay(500);
    handle_ballgrabber(); // grab the ball
    turn_around_right(); // turn around to face the opposite direction
    CURRENT_CASE_PID = 3; // set PID case to 0
    delay(500);
    move_forward_different(100, 0, 1); // move forward to the middle of the cell
    NeutralSensorValues = original_calibration_values; // reset sensor values
    CURRENT_CASE_PID = 4;
    pid_move_function(50);
    delay(500);
    CURRENT_CASE_PID = 2; // set PID case to 0
    rotate_right(); // rotate right to face the exit of starting area
    delay(500);
    move_forward_different(100, 0, 1.5); // move to the exit of starting area
    delay(500);
    CURRENT_CASE_PID = 4;
    pid_move_function(50);
    delay(500);
} 