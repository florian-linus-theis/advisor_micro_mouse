#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"


// Global variables 
int current_option = MODE_STANDBY;
int selected_option = MODE_STANDBY;
bool optionSelected = false;
bool encoderTurned = false;
bool confirmationPending = false;

void setup() {
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

void loop() {
    // Check if the encoder was turned
    if (encoderTurned) {
        encoderTurned = false;
        updateEncoderState(); 
        displayOptions(static_cast<Mode>(selected_option), confirmationPending);
    }

    if (optionSelected) {
        optionSelected = false;
        if (confirmationPending) {
            confirmationPending = false;
            current_option = selected_option;
            handleModeSelection(static_cast<Mode>(current_option));
        } else {
            confirmationPending = true;
            displayOptions(static_cast<Mode>(selected_option), confirmationPending);
        }
    }

    // Sleep to reduce CPU usage (adjust as necessary)
    delay(100);
}
