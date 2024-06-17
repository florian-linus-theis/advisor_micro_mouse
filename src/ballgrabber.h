#pragma once
#include <Setup/Setup.h>


// Für Servo 
// #define TIMER_FREQUENCY 142000000 // 168 MHz

// Servo pulse widths (in microseconds)
const uint16_t PULSE_WIDTH_0 = 500; // 0.5 ms
const uint16_t PULSE_WIDTH_180 = 2500; // 0.5 ms
const uint16_t DEGREE_FRONT = 110;
const uint16_t DEGREE_BACK = 0;
const uint16_t DEGREE_GROUND = 140;

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
    writeServo(DEGREE_FRONT);
}

// Function to move the ball grabber backward (above robot)
void move_ballgrabber_backward() {
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
    // drive toward north edge of square (0,1)
    drive_forward(SPEED_MAPPING, SPEED_MAPPING, ACCELERATION_NORMAL, 1.778);
    // drive right curve 
    curve_right();
    // drive one forward
    drive_forward(SPEED_MAPPING, 0, ACCELERATION_NORMAL, 1.05);
    rotate_right();
    // backup to wall 
    BackwardBoth(90);
    delay(20); // wait for motors to react
    while(current_avg_speed < 0){};
    delay(200); // wait to make sure we are positioned nicely along the wall
    stop();
    reset_distance_traveled_after_straight();
    delay(50);
    // timer14->pause();
    timer10->resume();
    digitalWrite(SERVO_ENABLE, HIGH);
    // move ballgrabber forward
    move_ballgrabber_forward();
    // drive to ball 
    reset_PID_values(); 
    drive_forward(200, 0, ACCELERATION_NORMAL, 0.24); 
    delay(400);
    // move ballgrabber backwards
    move_ballgrabber_backward();
    delay(400);
    digitalWrite(SERVO_ENABLE, LOW);
    timer10->pause();
    // rotate right 
    rotate_right();
    reset_PID_values();
    // move back to exit 
    drive_forward(SPEED_MAPPING, SPEED_MAPPING, ACCELERATION_NORMAL, 0.95);
    curve_right(); // now at northern edge of cell (0, 2)
} 