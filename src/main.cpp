//Header-Files Include
#include "Pin_Setup.h"
#include "Clock_Setup.h"
#include "ADC_Setup.h"
#include "Timer_Setup.h"
#include "Systick.h"
#include "algorithms.cpp"

//Library-Include
#include <Arduino.h>

// put function declarations here:



void setup() {
  //Setup
  Pin_Setup();
  Clock_Setup();
  Timer_Setup();
  ADC_Setup();
  maze_setup(); // setting up the maze file
  HardwareSerial Serial1(BLUETOOTH_RX, BLUETOOTH_TX);
  Serial1.begin(115200);


//Start Systick Timer
  timer14.resume();
}


void loop() {
  // Check if the encoder was turned
    if (encoderTurned) {
        encoderTurned = false;
        updateEncoderState(); 
        displayOptions(static_cast<Mode>(selected_option), confirmationPending);
    }

    // Check if an option/state was selected
    if (optionSelected) {
        optionSelected = false;
        if (confirmationPending) {
            confirmationPending = false;
            current_option = selected_option;
            handleModeSelection(static_cast<Mode>(current_option));
        } else {
            confirmationPending = true;
            displayOptions(static_cast<Mode>(selected_option), true);
        }
    }

    // Sleep to reduce CPU usage (adjust as necessary)
    delay(100);
}

//put funktions here