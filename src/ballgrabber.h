#pragma once
#include <Setup.h>
#include <Arduino.h>

const int minPulseWidth = 544;  // Minimum pulse width in microseconds
const int maxPulseWidth = 2400; // Maximum pulse width in microseconds
const int servoFrequency = 50;  // Servo PWM frequency in Hz



//Function to write an angle to the servo
// void writeServo(int angle) {
//     // Map the angle to the pulse width
//     int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);
//     // Calculate the duty cycle for the desired pulse width
//     int dutyCycle = (pulseWidth * 4096) / (1000000 / servoFrequency);
//     analogWrite(SERVO_PWM_1, dutyCycle);
// }


void writeServo(int angle) {
    // Map the angle to the pulse width
    int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);
    float percentage = pulseWidth / 180 * 100;

    // Update the PWM duty cycle
    timer10->setCaptureCompare(1, percentage, PERCENT_COMPARE_FORMAT);
    timer10->refresh();
}



// Function to move the ball grabber forward
void move_ballgrabber_forward() {
    writeServo(180);
}

// Function to move the ball grabber backward
void move_ballgrabber_backward() {
    writeServo(0);
}

// Function to move the ball grabber to the middle position
void move_ballgrabber_middle() {
    writeServo(90);
}



void handle_ballgrabber(){
    digitalWrite(LED_GREEN, LOW);

    move_ballgrabber_forward();
    delay(2000);
    move_ballgrabber_middle();
    delay(2000);
    move_ballgrabber_backward();
    delay(2000);

    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);

    // while (true) {
    //     // // Sweep the servo back and forth
    //     // for (int angle = 0; angle <= 180; angle += 10) {
    //     //     writeServo(SERVO_PWM_1, angle);  // Move servo to the current angle
    //     //     delay(500);  // Wait for the servo to reach the position
    //     // }
    //     // for (int angle = 180; angle >= 0; angle -= 10) {
    //     //     writeServo(SERVO_PWM_1, angle);  // Move servo to the current angle
    //     //     delay(500);  // Wait for the servo to reach the position
    //     // }
    // }
}