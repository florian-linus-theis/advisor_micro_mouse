#pragma once 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>
#include <robin.h>
#include <Setup.h>
#include <ballgrabber.h>


// Define screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Initialize the OLED display using the Wire library -> I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


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

// declaring Global variables from main file 
extern int current_option;
extern int selected_option;
extern bool optionSelected;
extern bool encoderTurned;
extern bool confirmationPending;

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
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setRotation(2); 
    display.display();

    display.setCursor(0, 0);
    if (confirmation) {
        display.println("Press to Confirm:");
    } else {
        display.println("Available Modes:");
    }

    for (int i = 0; i < numOptions; ++i) {
        if (i == currentMode) {
            display.print(" >");
        } else {
            display.print(" ");
        }
        display.print(options[i]);
        display.print(" ");
        if (i == 2) {
            display.println();
        }
    }
    // Display the buffer on the screen
    display.display();
}

void display_print(std::string text, int text_size = 1){
    display.clearDisplay();
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(text.c_str());
    display.display();
}

// Function to handle the selected mode
void handleModeSelection(Mode mode) {
    display.clearDisplay();
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
            display_print("Data Mode selected");
            delay(1000);
            // Handle Hard Reset Mode
            break;
        case MODE_MAP_MAZE:
            display_print("DFS Mode selected");
            delay(1000);
            robin_test();
            // Handle Map Maze Mode
            break;
        case MODE_BFS:
            display_print("BFS Mode selected");
            delay(1000);
            // Handle BFS Mode
            break;
        case MODE_ASTAR:
            display_print("A* Mode selected");
            delay(1000);
            // Handle A* Mode
            break;
        default:
            display_print("Invalid mode");
            break;
    }
}

void display_setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // So that Mouse does not turn off
    pinMode(PC10, OUTPUT);
    digitalWrite(PC10, HIGH);

    // Initialize the I2C bus for Display communication with wire library
    Wire.setSCL(OLED_SCL);
    Wire.setSDA(OLED_SDA);
    Wire.begin();

    // Initialize the OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }

    // Clear the buffer
    display.clearDisplay();

    // Attach interrupts
    attachInterrupt(digitalPinToInterrupt(UI_ENCODER_A), handleEncoderTurn, CHANGE);
    attachInterrupt(digitalPinToInterrupt(UI_ENCODER_B), handleEncoderTurn, CHANGE);
    attachInterrupt(digitalPinToInterrupt(UI_BUTTON), handleEncoderButton, FALLING);

    // Display the initial options
    displayOptions(static_cast<Mode>(current_option), false);
}
