#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Define OLED reset pin
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// TODO: Hannes Rotary Encoder Pins
#define ENCODER_PIN_A PH0 // Example pin, change to match your STM32 setup
#define ENCODER_PIN_B PC15 // Example pin, change to match your STM32 setup
#define ENCODER_BUTTON_PIN PC12 // Example pin, change to match your STM32 setup

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

// Globals
volatile int currentOption = MODE_STANDBY;
volatile bool optionSelected = false;
volatile bool encoderTurned = false;
volatile bool confirmationPending = false;

// Function prototypes
void displayOptions(Mode currentMode, bool confirmation);
void handleModeSelection(Mode mode);
void handleEncoderTurn();
void handleEncoderButton();
void updateEncoderState();

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(9600);

    // Initialize the OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }

    // Clear the buffer
    display.clearDisplay();

    display.setTextSize(1); // Change this to 2, 3, etc., to increase the font size
    display.setTextColor(SSD1306_WHITE);

    // Initialize rotary encoder pins
    pinMode(ENCODER_PIN_A, INPUT);
    pinMode(ENCODER_PIN_B, INPUT);
    pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);

    // Attach interrupts
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoderTurn, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), handleEncoderTurn, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON_PIN), handleEncoderButton, FALLING);

    // Display the initial options
    displayOptions(static_cast<Mode>(currentOption), false);
}

void loop() {
    if (encoderTurned) {
        encoderTurned = false;
        updateEncoderState();
        displayOptions(static_cast<Mode>(currentOption), confirmationPending);
    }

    if (optionSelected) {
        optionSelected = false;
        if (confirmationPending) {
            confirmationPending = false;
            handleModeSelection(static_cast<Mode>(currentOption));
        } else {
            confirmationPending = true;
            displayOptions(static_cast<Mode>(currentOption), true);
        }
    }

    // Sleep to reduce CPU usage (adjust as necessary)
    delay(100);
}

// Interrupt Service Routine (ISR) for rotary encoder turn
void handleEncoderTurn() {
    encoderTurned = true;
    // For later purposes
    // if (currentOption != MODE_STANDBY && conformationPending = false && optionSelected = false) {
    //     currentOption = MODE_STANDBY;
    // }
}

// Interrupt Service Routine (ISR) for rotary encoder button press
void handleEncoderButton() {
    optionSelected = true;
}

// Update the encoder state
void updateEncoderState() {
    static int lastEncoded = 0;
    int MSB = digitalRead(ENCODER_PIN_A); // Most significant bit
    int LSB = digitalRead(ENCODER_PIN_B); // Least significant bit
    int encoded = (MSB << 1) | LSB; // Combine the two pin values into a single integer
    int sum = (lastEncoded << 2) | encoded; // Append the previous state to the current state

    // Determine the direction based on state changes
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        currentOption = (currentOption + 1) % MODE_MAX;
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        currentOption = (currentOption - 1 + MODE_MAX) % MODE_MAX;
    }

    lastEncoded = encoded; // Store the current state for the next iteration

    // If the encoder is turned while waiting for confirmation, cancel confirmation
    if (confirmationPending) {
        confirmationPending = false;
    }
}

// Function to display available options on the OLED screen
void displayOptions(Mode currentMode, bool confirmation) {
    const char* options[] = {"Stand By", "Soft Reset", "Hard Reset", "Map Maze", "BFS", "A*"};
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
            display.print("> ");
        } else {
            display.print("  ");
        }
        display.println(options[i]);
    }
    // Display the buffer on the screen
    display.display();
}

// Function to handle the selected mode
void handleModeSelection(Mode mode) {
    switch (mode) {
        case MODE_STANDBY:
            Serial.println("Stand By Mode selected");
            // Handle Stand By Mode
            break;
        case MODE_SOFT_RESET:
            Serial.println("Soft Reset Mode selected");
            // Handle Soft Reset Mode
            break;
        case MODE_HARD_RESET:
            Serial.println("Hard Reset Mode selected");
            // Handle Hard Reset Mode
            break;
        case MODE_MAP_MAZE:
            Serial.println("Map Maze Mode selected");
            // Handle Map Maze Mode
            break;
        case MODE_BFS:
            Serial.println("BFS Mode selected");
            // Handle BFS Mode
            break;
        case MODE_ASTAR:
            Serial.println("A* Mode selected");
            // Handle A* Mode
            break;
        default:
            Serial.println("Invalid mode");
            break;
    }
}
