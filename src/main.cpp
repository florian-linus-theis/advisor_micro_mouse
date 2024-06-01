//Include Header Files
//#pragma once
#include "Setup\Setup.h"

//Include Librarys in Setup.h !!!

#include "algorithms.h"
#include <robin.h>
#include <ballgrabber.h>



// Global variables 
int current_option = MODE_STANDBY;
int selected_option = MODE_STANDBY;
bool optionSelected = false;
bool encoderTurned = false;
bool confirmationPending = false;



void setup() {
    Pin_Setup();
    Timer_Setup();
    ADC_Setup();
    maze_setup(); // setting up the maze file
    set_Interrupt_Priority();

    //Start Systick Timer
    timer14->resume();

    // Setup Complete
    // Display the initial options on display
    displayOptions(static_cast<Mode>(current_option), false);

    // Test Bluetooth
    ble->println("test bluetooth");
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
}