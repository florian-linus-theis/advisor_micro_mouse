#pragma once 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>


// Define screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Define OLED reset pin
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_SDA_PIN PB11 // Define OLED SDA Pin
#define OLED_SCL_PIN PB10 // Define OLED SCL Pin

// Initialize the OLED display using the Wire library -> I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define Rotary Encoder Pins
#define ENCODER_PIN_A PH0 
#define ENCODER_PIN_B PC15 
#define ENCODER_BUTTON_PIN PC12  


// Define an enum for all modes
enum Mode {
    MODE_STANDBY,
    MODE_SOFT_RESET,
    MODE_HARD_RESET,
    MODE_MAP_MAZE,
    MODE_BFS,
    MODE_ASTAR,
    MODE_MAX // This can be used to determine the number of modes
};

// declaring Global variables from main file 
int current_option = MODE_STANDBY;
int selected_option = MODE_STANDBY;
bool optionSelected = false;
bool encoderTurned = false;
bool confirmationPending = false;

// Rotary encoder debouncing variables
volatile unsigned long lastTurnTime = 0;
const unsigned long debounceDelay = 5; // Debounce delay in milliseconds


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
    static int lastEncoded = 0; // Store the last state of the encoder, static to retain value between function calls
    int MSB = digitalRead(ENCODER_PIN_A); // Most significant bit
    int LSB = digitalRead(ENCODER_PIN_B); // Least significant bit
    int encoded = (MSB << 1) | LSB; // Combine the two pin values into a single integer
    int sum = (lastEncoded << 2) | encoded; // Append the previous state to the current state

    // Determine the direction based on state changes and update the selected option
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        selected_option = (selected_option + 1) % MODE_MAX;
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        selected_option = (selected_option - 1 + MODE_MAX) % MODE_MAX;
    }

    lastEncoded = encoded; // Store the current state for the next iteration

    // If the encoder is turned while waiting for confirmation, cancel confirmation
    if (confirmationPending) {
        confirmationPending = false;
    }
}

// Function to display available options on the OLED screen
void displayOptions(Mode currentMode, bool confirmation) {
    const char* options[] = {"S-B", "S-Res", "H-Res", "DFS", "BFS", "A*"};
    int numOptions = MODE_MAX;

    // Clear the display buffer
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    if (confirmation) {
        display.println("Press again to confirm:");
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
        display.print(", ");
        if (i == 2) {
            display.println();
        }
    }
    // Display the buffer on the screen
    display.display();
}

// Function to handle the selected mode
void handleModeSelection(Mode mode) {
    display.clearDisplay();
    switch (mode) {
        case MODE_STANDBY:
            display.println("Stand By Mode selected");
            delay(1000); // Delay to allow the user to read the message
            // Handle Stand By Mode
            break;
        case MODE_SOFT_RESET:
            display.println("Soft Reset Mode selected");
            delay(1000);
            // Handle Soft Reset Mode
            break;
        case MODE_HARD_RESET:
            display.println("Hard Reset Mode selected");
            delay(1000);
            // Handle Hard Reset Mode
            break;
        case MODE_MAP_MAZE:
            display.println("Map Maze Mode selected");
            delay(1000);
            // Handle Map Maze Mode
            break;
        case MODE_BFS:
            display.println("BFS Mode selected");
            delay(1000);
            // Handle BFS Mode
            break;
        case MODE_ASTAR:
            display.println("A* Mode selected");
            delay(1000);
            // Handle A* Mode
            break;
        default:
            display.println("Invalid mode");
            break;
    }
}

void display_print(std::string text, int text_size = 1){
    display.clearDisplay();
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(text.c_str());
    display.display();
}

void display_setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // So that Mouse does not turn off
    pinMode(PC10, OUTPUT);
    digitalWrite(PC10, HIGH);

    // Initialize the I2C bus for Display communication with wire library
    Wire.setSCL(OLED_SCL_PIN);
    Wire.setSDA(OLED_SDA_PIN);
    Wire.begin();

    // Initialize the OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }

    // Clear the buffer
    display.clearDisplay();

    // Initialize rotary encoder pins
    pinMode(ENCODER_PIN_A, INPUT);
    pinMode(ENCODER_PIN_B, INPUT);
    pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);

    // Attach interrupts
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoderTurn, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), handleEncoderTurn, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON_PIN), handleEncoderButton, FALLING);

    // Display the initial options
    displayOptions(static_cast<Mode>(current_option), false);
}