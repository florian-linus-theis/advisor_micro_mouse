//#pragma once
//Include Header Files
#include "Setup\Setup.h"

//Include C-Files
#include "algorithms.h"
#include "display.h"
#include <robin.h>
#include <ballgrabber.h>

//Library-Include in Setup.h Header File


// put external variable declarations here:
/*
extern int current_option;
extern int selected_option;
extern bool optionSelected;
extern bool encoderTurned;
extern bool confirmationPending;
*/

// Global variables 
int current_option = MODE_STANDBY;
int selected_option = MODE_STANDBY;
bool optionSelected = false;
bool encoderTurned = false;
bool confirmationPending = false;



// put function declarations in Header Files




void setup() {
    //Setup
    //Clock_Setup();  
    Pin_Setup();
    Timer_Setup();
    ADC_Setup();
    maze_setup(); // setting up the maze file
    display_setup();

    // Display the initial options on display
    displayOptions(static_cast<Mode>(current_option), false);

    //Start Systick Timer
    // timer14.resume();
  
    // Robin's Code
    // Sensor_Sync_Setup();
    // Mid_Sensor_Setup();
    // ble->println("test1");
    // Timer2_Setup();
    // Timer3_Setup();
    // Timer4_Setup();
    // ble->println("test2");
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
    // Distanz_Messung_Sensoren();
    // digitalWrite(LED_RED, LOW);
    // printDistanzSensoren();

    // Sleep to reduce CPU usage (adjust as necessary)
    //delay(100);
}
