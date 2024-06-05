#pragma once
#include "Setup\Setup.h"
#include <robin.h>
#include <ballgrabber.h>
#include "PID_neu.h"
#include "Music.h"


// Define an enum for all modes
enum Mode {
    MODE_STANDBY,
    MODE_SOFT_RESET,
    MODE_SHOW_DATA,
    MODE_MAP_MAZE,
    MODE_BFS,
    MODE_ASTAR,
    MODE_MAX // This can be used to determine the number of modes
};

// Rotary encoder debouncing variables
volatile unsigned long lastTurnTime = 0;
const unsigned long debounceDelay = 100; // Debounce delay in milliseconds

volatile int buzzer_counter;


// Functions
// Interrupt Service Routine (ISR) for rotary encoder turn
void handleEncoderTurn() {
    if ((millis() - lastTurnTime) > debounceDelay) {
        encoderTurned = true;
        lastTurnTime = millis(); // Update the last turn time
    }
    // check if we are inside standby mode and reset the current option to standby mode
    if (current_option != MODE_STANDBY && confirmationPending == false && optionSelected == false) {
        current_option = MODE_STANDBY; // change to standby mode (other functions will regularly check for this value)
    }
}


// Interrupt Service Routine (ISR) for rotary encoder button press
void handleEncoderButton() {
    optionSelected = true;
    Buzzer_beep_noBlock(1000, 1, 100);
}


// Update the encoder state
void updateEncoderState() {
    int right = digitalRead(UI_ENCODER_A);
    int left = digitalRead(UI_ENCODER_B);
    if (right == 0) {
        selected_option = (selected_option + 1) % MODE_MAX;
    } else {
        selected_option = (selected_option - 1 + MODE_MAX) % MODE_MAX;
    }
    if (confirmationPending) {
        confirmationPending = false;
    }
}


// Function to display available options on the OLED screen
void displayOptions(Mode currentMode, bool confirmation) {
    const char* options[] = {"S-B", "S-Res", "Data", "DFS", "BFS", "A*"};
    int numOptions = MODE_MAX;

    // Clear the display buffer
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setRotation(2);
    display->display();

    display->setCursor(0, 0);
    if (confirmation) {
        display->println("Press to Confirm:");
    } else {
        display->println("Available Modes:");
    }

    for (int i = 0; i < numOptions; ++i) {
        if (i == currentMode) {
            display->print(" >");
        } else {
            display->print(" ");
        }
        display->print(options[i]);
        display->print(" ");
        if (i == 2) {
            display->println();
        }
    }
    // Display the buffer on the screen
    display->display();
}


void display_print(std::string text, int text_size = 1){
    display->clearDisplay();
    display->setTextSize(text_size);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->println(text.c_str());
    display->display();
}


// Function to handle the selected mode
void handleModeSelection(Mode mode) {
    display->clearDisplay();
    switch (mode) {
        case MODE_STANDBY:
            display_print("Stand By Mode selected");
            delay(1000); // Delay to allow the user to read the message
            calibrate_sensors(10, 10);
            // Handle Stand By Mode
            break;
        case MODE_SOFT_RESET:
            display_print("Soft Reset Mode selected");
            timer14->resume(); // starting systick timer
            delay(1000);
            // ForwardBoth(10);
            move_actual(100);
            delay(2000);
            ForwardRight(0);
            ForwardLeft(0);
            //stop();
            delay(1000);
            digitalWrite(MOTOR_ENABLE, HIGH); // disable motor
            timer14->pause(); // stopping systick timer
            delay(1000);
            break;
        case MODE_SHOW_DATA:
            // display_print("Data Mode selected");
            // // delay(1000);
            // display_print("Testing Encoders");
            timer14->resume(); // starting systick timer
            delay(1000);
            digitalWrite(MOTOR_ENABLE, LOW); // enable motor
            delay(1000);
            grab_ball();
            timer14->pause();
            display->clearDisplay();
            delay(100);
            display->println("Test Servo");
            // Handle Hard Reset Mode
            delay(100);
            break;
        case MODE_MAP_MAZE:
            display_print("DFS Mode selected");
            digitalWrite(MOTOR_ENABLE, LOW); // enable motor
            timer14->resume(); // starting systick timer
            delay(1000);
            left_curve(100);
            left_curve(100);
            left_curve(100);
            left_curve(100);
            delay(500);
            // decelerate();
            // right_curve(DUTY_SLOW);
            // stop();
            digitalWrite(MOTOR_ENABLE, HIGH); // disable motor
            timer14->pause(); // stopping systick timer
            delay(200);
            // ble->println(avg_distance_traveled);
            // Handle Map Maze Mode
            break;
        case MODE_BFS:
            timer14->resume(); // starting systick timer
            display_print("BFS Mode selected");
            PID_Test();
            timer14->pause();
            delay(1000);
            //Imperial_March();
            // Handle BFS Mode
            break;
        case MODE_ASTAR:
            delay(1000);
            timer14->resume(); // starting systick timer
            digitalWrite(MOTOR_ENABLE, LOW); // enable motor
            display_print("A* Mode selected");
            // resetting all values to zero to ensure no previous values are used and no beginning encoder values read
            reset_encoders();
            reset_PID_values();
            // Handle A* Mode
            move_forward_different(200, 0, 4);
            delay(500);
            rotate_right();
            delay(500);
            rotate_right();
            delay(500);
            move_forward_different(200, 0, 4);
            timer14->pause();
            display_print("A* Mode completed");
            digitalWrite(MOTOR_ENABLE, HIGH); // disable motor
            break;
        default:
            display_print("Invalid mode");
            break;
    }
}

void Buzzer_beep(int freq, int beeps) {  //Frequency and Number of beeps
    int overflow = 1000000/freq;
    timer1->setOverflow(overflow);
    timer1->setCaptureCompare(4, overflow/2, TICK_COMPARE_FORMAT);
    timer1->refresh();

    int i = 1;
    while(true) {
        timer1->resume();
        delay(100);
        timer1->pause();
        if(i >= beeps) break;
        delay(100);
        i++;
    }
}

void Buzzer_beep(int freq, int beeps, int length) {  //Frequency, Number of beeps and Tone Length in ms
    int overflow = 1000000/freq;
    timer1->setOverflow(overflow);
    timer1->setCaptureCompare(4, overflow/2, TICK_COMPARE_FORMAT);
    timer1->refresh();

    int i = 1;
    while(true) {
        timer1->resume();
        delay(length);
        timer1->pause();
        if(i >= beeps) break;
        delay(length);
        i++;
    }
}


void Buzzer_beep_noBlock(int freq, int beeps, int length) {  //Frequency in Hz, Number of beeps and Tone Length in ms
    int overflow = 1000000/freq;
    timer1->setOverflow(overflow);
    timer1->setCaptureCompare(4, overflow/2, TICK_COMPARE_FORMAT);  // 50% Duty Cycle - square wave
    timer1->refresh();
    timer1->resume();

    buzzer_counter = (beeps * 2) - 2;
    timer7->setOverflow(length);
    timer7->refresh();
    timer7->resume();
}


void Timer7_Interrupt(void) { 
    if ((buzzer_counter)%2 == 0) {
        timer1->pause();
        if(buzzer_counter == 0) timer7->pause();
    }
    else {
        timer1->resume();
    }
    buzzer_counter--;
}



// calibrate the sensors - first in the air, then in the maze start cell.
void calibrate_sensors(int measurements_air, int measurements_maze){
    int air_values[7];
    int maze_values[7];

    digitalWrite(LED_GREEN, LOW);
    Buzzer_beep(3000, 3, 100);


    //meassure in the air and add up to array
    for (int i=0; i<measurements_air; i++){
        Distanz_Messung_Sensoren();
        for(int i=0; i<7; i++) {
            air_values[i] += Distance_Sensor[i];
        }
    }
    digitalWrite(LED_GREEN, LOW);

    start();

    //meassure in the maze and add up to array
    for (int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        for(int i=0; i<7; i++) {
            maze_values[i] += Distance_Sensor[i];
        }
    }

    // calculate average by dividing through # of meassurements
    for(int i=0 ; i<7 ; i++){
        calibration_sensor[i] = static_cast<int>(std::round(static_cast<double>(air_values[i]) / measurements_maze));
        NeutralSensorValues[i] = static_cast<int>(std::round(static_cast<double>(maze_values[i]) / measurements_maze));
    }
}



void start(){
    while(Distance_Sensor[5] <= 1500){   //SENSOR_RD
        Distanz_Messung_Sensoren();
        delay(50);
    }

    digitalWrite(LED_GREEN, LOW);       //SENSOR_RD
    while(Distance_Sensor[5] > 1500){
        Distanz_Messung_Sensoren();
        delay(50);
    }

    delay(200);
    Buzzer_beep(3000, 3, 100);
    digitalWrite(LED_GREEN, HIGH);

    //timer14->resume();
    //move_forward_different(100, 100, 0.5);
}