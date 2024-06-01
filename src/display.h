#pragma once
#include <Setup\Setup.h>
#include <robin.h>
#include <ballgrabber.h>


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
            // Handle Stand By Mode
            break;
        case MODE_SOFT_RESET:
            display_print("Soft Reset Mode selected");
            delay(1000);
            handle_ballgrabber(); // Function to handle the ball grabber 
            // Handle Soft Reset Mode
            break;
        case MODE_SHOW_DATA:
            // display_print("Data Mode selected");
            // // delay(1000);
            // display_print("Testing Encoders"); 
            display->clearDisplay();
            display->println("Test");
            test_encoders();
            // Handle Hard Reset Mode
            break;
        case MODE_MAP_MAZE:
            display_print("DFS Mode selected");
            delay(1000);
            timer14->resume(); // starting systick timer
            robin_test();
            // Handle Map Maze Mode
            break;
        case MODE_BFS:
            display_print("BFS Mode selected");
            delay(1000);
            Buzzer_beep(2000, 4);
            // Handle BFS Mode
            break;
        case MODE_ASTAR:
            display_print("A* Mode selected");
            digitalWrite(SERVO_ENABLE, LOW);

            // Code doesnt work YET
            // Timer4_Setup_Motor(); 
            ble->println("motor setup done");
            // delay(2000);
            robin_test();
            delay(500);
            ble->println("robin test done");
            // digitalWrite(MOTOR_ENABLE, HIGH); // disable motor
            Timer4_Setup_Servo();
            digitalWrite(SERVO_ENABLE, HIGH);
            delay(500); // delay to allow the user to read the message
            ble->println("servo setup done");
            timer4->setCaptureCompare(3, 500, MICROSEC_COMPARE_FORMAT); // 0 degrees
            // timer4->refresh();
            delay(1000);
            timer4->setCaptureCompare(3, 1950, MICROSEC_COMPARE_FORMAT); // 180 degrees
            // delay(2000);
            delay(750);
            digitalWrite(SERVO_ENABLE, LOW);
            ble->println("ballgrabber done");
            Timer4_Setup_Motor();
            // // delay(1000);
            ble->println("motor setup done");
            robin_test();
            // Handle A* Mode
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

    for(int i=0; i<beeps; i++) {
        timer1->resume();
        delay(100);
        timer1->pause();
        delay(100);
    }
}