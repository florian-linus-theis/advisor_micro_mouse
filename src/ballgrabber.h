#pragma once
#include <Setup.h>
#include <Arduino.h>

const int servoPin = SERVO_PWM_1;  // Pin to which the servo is connected
const int minPulseWidth = 544;  // Minimum pulse width in microseconds
const int maxPulseWidth = 2400;  // Maximum pulse width in microseconds
const int servoFrequency = 50;  // Servo PWM frequency in Hz

// void setup_servo() {
//     pinMode(SERVO_PWM_1, OUTPUT);  // Set the servo pin as output
//     analogWriteFrequency(SERVO_PWM_1, servoFrequency);  // Set the PWM frequency
// }

// // Function to write an angle to the servo
// void writeServo(int angle) {
//     // Map the angle to the pulse width
//     int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);
//     // Calculate the duty cycle for the desired pulse width
//     int dutyCycle = (pulseWidth * 4096) / (1000000 / servoFrequency);
//     analogWrite(SERVO_PWM_1, dutyCycle);
// }

// void setupPWM(int pin) {
//     // Set the PWM frequency to 50Hz
//     timer10->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, pin);
//     timer10->setOverflow(20000, HERTZ_FORMAT);  // 20ms period (50Hz)
//     timer10->setCaptureCompare(1, 1500, MICROSEC_COMPARE_FORMAT);  // Center the servo

//     timer10->resume();
// }

// void setup_servo() {
//     // Set up the pin as PWM output
//     pinMode(servoPin, OUTPUT);
//     setupPWM(servoPin);
// }



// void writeServo(int pin, int angle) {
//     // Map the angle to the pulse width
//     int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);

//     // Update the PWM duty cycle

//     timer10->setCaptureCompare(1, pulseWidth, MICROSEC_COMPARE_FORMAT);
// }


// // // Function to move the ball grabber forward
// // void move_ballgrabber_forward() {
// //     writeServo(180);
// // }

// // // Function to move the ball grabber backward
// // void move_ballgrabber_backward() {
// //     writeServo(0);
// // }

// // // Function to move the ball grabber to the middle position
// // void move_ballgrabber_middle() {
// //     writeServo(90);
// // }

// void handle_ballgrabber(){
//     while (true) {
//         // Sweep the servo back and forth
//         for (int angle = 0; angle <= 180; angle += 10) {
//             writeServo(servoPin, angle);  // Move servo to the current angle
//             delay(500);  // Wait for the servo to reach the position
//         }
//         for (int angle = 180; angle >= 0; angle -= 10) {
//             writeServo(servoPin, angle);  // Move servo to the current angle
//             delay(500);  // Wait for the servo to reach the position
//         }
//     }
// }