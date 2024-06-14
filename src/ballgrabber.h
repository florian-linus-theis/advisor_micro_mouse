#pragma once
#include <Setup/Setup.h>


// Für Servo 
// #define TIMER_FREQUENCY 142000000 // 168 MHz

// Servo pulse widths (in microseconds)
const uint16_t PULSE_WIDTH_0 = 500; // 0.5 ms
const uint16_t PULSE_WIDTH_180 = 2500; // 0.5 ms
const uint16_t DEGREE_FRONT = 8;
const uint16_t DEGREE_BACK = 130;

// Sensor Calibrations 
std::vector<int> ballgrabber_calibration(7, 0);


void writeServo(int angle) {
    // Update the PWM duty cycle
    int decimal = constrain(map(angle, 0, 180, PULSE_WIDTH_0, PULSE_WIDTH_180), PULSE_WIDTH_0, PULSE_WIDTH_180);
    timer10->setCaptureCompare(1, decimal, TICK_COMPARE_FORMAT);
    timer10->refresh();
    timer10->resume();
}


// Function to move the ball grabber forward (in front of robot)
void move_ballgrabber_forward() {
    for (int degree = DEGREE_BACK; degree >= DEGREE_FRONT; degree--) {
        writeServo(degree);
        delay(100);
    }
    writeServo(DEGREE_FRONT);
}

// Function to move the ball grabber backward (above robot)
void move_ballgrabber_backward() {
    // delay(220);
    // writeServo(DEGREE_BACK);
    // delay(1000);
    for (int degree = DEGREE_FRONT; degree <= DEGREE_BACK; degree++) {
        writeServo(degree);
        delay(40);
    }
    writeServo(DEGREE_BACK);
}


//handling ballgrabber action including reset of motor timer and servo timer
void handle_ballgrabber(){
    // digitalWrite(MOTOR_ENABLE, HIGH);
    digitalWrite(SERVO_ENABLE, HIGH);
    move_ballgrabber_forward();
    move_ballgrabber_backward();
    digitalWrite(SERVO_ENABLE, LOW);
}

// Full Function to grab the ball
void grab_ball(){
    // drive toward enorth dge of square (0,1)
    // timer10->pause();
    // drive_forward(365, 365, 1.778);
    // // drive right curve 
    // curve_right();
    // // drive one forward
    // drive_forward(365, 0, 0.9);
    // rotate_right();
    // // backup to wall 
    // BackwardBoth(90);
    // delay(30);
    // while(current_avg_speed < 0){};
    // delay(200);
    // stop();
    // delay(1000);
    // timer14->pause();
    timer10->resume();
    ble->println("Ballgrabber enabled");
    digitalWrite(SERVO_ENABLE, HIGH);
    ble->println("Servo enabled");
    // move ballgrabber forward
    move_ballgrabber_forward();
    // drive to ball 
    reset_PID_values(); 
    timer10->pause();
    timer14->resume();
    // drive_forward(200, 0, 0.24); 
    timer14->pause();
    timer10->resume();
    delay(500);
    // move ballgrabber backwards
    move_ballgrabber_backward();
    digitalWrite(SERVO_ENABLE, LOW);
    // timer10->pause();
    // timer14->resume();
    // // rotate right 
    // rotate_right();
    reset_PID_values();
    // move back to exit 
    drive_forward(365, 365, 1);
    curve_right(); // now at northern edge of cell (0, 2)
} 