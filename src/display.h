#pragma once
#include "Setup\Setup.h"
#include <robin.h>
#include <ballgrabber.h>
//#include "battery.h"
#include "PID.h"
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
            display_print("Calibration Mode selected");
            delay(1000); // Delay to allow the user to read the message
            calibrate_sensors(10, 10);
            ble->println("Calibration done");
            ble->println("Calibration values: ");
            ble->println("0:" + String(calibration_sensor[0]) + " 1:" + String(calibration_sensor[1]) + " 2:" + String(calibration_sensor[2]) + " 3:" + String(calibration_sensor[3]) + " 4:" + String(calibration_sensor[4]) + " 5:" + String(calibration_sensor[5]) + " 6:" + String(calibration_sensor[6]));
            ble->println("Neutral values: ");
            ble->println("0:" + String(NeutralSensorValues[0]) + " 1:" + String(NeutralSensorValues[1]) + " 2:" + String(NeutralSensorValues[2]) + " 3:" + String(NeutralSensorValues[3]) + " 4:" + String(NeutralSensorValues[4]) + " 5:" + String(NeutralSensorValues[5]) + " 6:" + String(NeutralSensorValues[6]));
            ble->println("Max values: ");
            ble->println("0:" + String(MaxSensorValues[0]) + " 1:" + String(MaxSensorValues[1]) + " 2:" + String(MaxSensorValues[2]) + " 3:" + String(MaxSensorValues[3]) + " 4:" + String(MaxSensorValues[4]) + " 5:" + String(MaxSensorValues[5]) + " 6:" + String(MaxSensorValues[6]));
            ble->println("Min values: ");
            ble->println("0:" + String(MinSensorValues[0]) + " 1:" + String(MinSensorValues[1]) + " 2:" + String(MinSensorValues[2]) + " 3:" + String(MinSensorValues[3]) + " 4:" + String(MinSensorValues[4]) + " 5:" + String(MinSensorValues[5]) + " 6:" + String(MinSensorValues[6]));   
            // always keep this last
            displayOptions(MODE_STANDBY, false);
            break;
        case MODE_SOFT_RESET:
            display_print("Soft Reset Mode selected");
            timer14->resume(); // starting systick timer
            delay(1000);
            SET_PID_MANUALLY = false;
            while(encoderTurned == false){}
            delay(1000);
            digitalWrite(MOTOR_ENABLE, HIGH); // disable motor
            timer14->pause(); // stopping systick timer
            // always keep this last
            displayOptions(MODE_SOFT_RESET, false);
            break;
        case MODE_SHOW_DATA:
            display_print("Data Mode selected");
            digitalWrite(MOTOR_ENABLE, LOW); //enable motor
            start(); 
            timer14->resume();
            delay(20);
            //resetting all values
            reset_encoders();
            reset_PID_values();
            while(1){
                //CURRENT_CASE_PID = 11;
                move_forward_different(100, 100, 1);
                distance_traveled_L_PID = 0;
                distance_traveled_R_PID = 0;
                // reset_distance_traveled();
                // move_forward_different(100, 0, 3);
                // CURRENT_CASE_PID = 4;
                // pid_move_function(50);
                // reset_PID_values();
                // delay(500);

                //encoder_based_move_function(100);
                delay(10);
                if(encoderTurned) break;
            }
            delay(500);
            timer14->pause();
            digitalWrite(MOTOR_ENABLE, HIGH); //disable motor
            delay(1000);
            display->clearDisplay();
            delay(1000);
           
            // always keep this last
            displayOptions(MODE_SHOW_DATA, false);
            break;
        case MODE_MAP_MAZE:
            display_print("DFS Mode selected");
            digitalWrite(MOTOR_ENABLE, LOW); // enable motor
            timer14->resume(); // starting systick timer
            delay(1000);
            start("right");
            SET_PID_MANUALLY = false;
            reset_encoders();
            reset_PID_values();
            move_forward_mapping(6);
            ble->println("Current Position: " "(" + String(cur_position[0]) + ", " + String(cur_position[1]) + ")");
            rotate_right();
            move_forward_mapping(1);
            ble->println("Current Position: " "(" + String(cur_position[0]) + ", " + String(cur_position[1]) + ")");
            rotate_right();
            move_forward_mapping(3);
            ble->println("Current Position: " "(" + String(cur_position[0]) + ", " + String(cur_position[1]) + ")");
            rotate_right();
            move_forward_mapping(1);
            move_forward_mapping(1);
            ble->println("Current Position: " "(" + String(cur_position[0]) + ", " + String(cur_position[1]) + ")");
            digitalWrite(MOTOR_ENABLE, HIGH); // disable motor
            timer14->pause(); // stopping systick timer
            delay(200);
            // ble->println(avg_distance_traveled);
            // Handle Map Maze Mode
            // always keep this last
            displayOptions(MODE_MAP_MAZE, false);
            break;
        case MODE_BFS:
             // starting systick timer
            digitalWrite(MOTOR_ENABLE, LOW); // enable motor
            display_print("BFS Mode selected");
            timer14->resume();
            start("right");
            reset_encoders();
            reset_PID_values();
            start("right");
            dfs_mapping();
            delay(1000);
            digitalWrite(MOTOR_ENABLE, HIGH); // disable motor
            timer14->pause(); // stopping systick timer
            //Imperial_March();
            // always keep this last
            displayOptions(MODE_BFS, false);
            break;
        case MODE_ASTAR:
            display_print("A* Mode selected wait for Finger");
            digitalWrite(MOTOR_ENABLE, LOW); // enable motor
            start("right"); // wait for finger
            timer14->resume(); // starting systick timer
            delay(200);
            // resetting all values to zero to ensure no previous values are used and no beginning encoder values read
            reset_encoders();
            reset_PID_values();
            drive_forward(365, 0, 3);
            timer14->pause();
            // display_print("A* Mode completed");
            digitalWrite(MOTOR_ENABLE, HIGH); // disable motor
            // always keep this last
            displayOptions(MODE_ASTAR, false);
            break;
        // default:
        //     display_print("Invalid mode");
        //     break;
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




// Function to start all driving modes 
// Waits for finger to be in front of the sensor (front right), then starts the driving mode
void start(){
    while(Distance_Sensor[6] <= 1250){   //SENSOR_RD
=======
void start(int sensor = 6){
    if (sensor < 0 || sensor > 6) return;
    while(Distance_Sensor[sensor] <= 1250){   //SENSOR_RD
        Distanz_Messung_Sensoren();
        delay(50);
    }
    digitalWrite(LED_GREEN, LOW);       //SENSOR_RD
    
    while(Distance_Sensor[sensor] > 1250){
        Distanz_Messung_Sensoren();
        delay(50);
    }

    delay(800);
    Buzzer_beep(4000, 2, 50);
    digitalWrite(LED_GREEN, HIGH);
}

void wait_for_other_side(std::string side = "right"){
    int i = 0;
    if (side == "right"){
        i = 5;
    }
    
    while(Distance_Sensor[i] <= 1250){   //SENSOR_RD
        Distanz_Messung_Sensoren();
        delay(50);
    }
}

