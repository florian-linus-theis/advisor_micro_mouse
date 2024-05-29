//#pragma once
//Include Header Files
#include "Setup.h"

//Include C-Files
#include "algorithms.h"
#include "display.h"
/*
#include "Pin_Setup.cpp"
#include "Clock_Setup.cpp"
#include "Timer_Setup.cpp"
#include "ADC_Setup.cpp"
#include "Systick.cpp"
#include "algorithms.h"
*/

//Library-Include in Setup.h Header File


// put external variable declarations here:
extern int current_option;
extern int selected_option;
extern bool optionSelected;
extern bool encoderTurned;
extern bool confirmationPending;

// put function declarations in Header Files




void setup() {
  //Setup  
  Pin_Setup();
  Clock_Setup();
  Timer_Setup();
  ADC_Setup();
  maze_setup(); // setting up the maze file
  display_setup();
}




void loop() {
  // Check if the encoder was turned
    /*if (encoderTurned) {
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
    delay(100);*/

    Distanz_Mid_Sensor();
    printDistanzSensoren();
    delay(200);
    /*
    while(Distance_Sensor_Mid_MM < 1500){
        Distanz_Mid_Sensor();
        printDistanzSensoren();
    }
    while(Distance_Sensor_Mid_MM > 1500){
        Distanz_Mid_Sensor();
        printDistanzSensoren();
    }
    delay(1000);
    while(Distance_Sensor_Mid_MM < 1500){
        Distanz_Mid_Sensor();
        Forward(10);
        printDistanzSensoren();
    }
    */
}

//put funktions here