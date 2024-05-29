//Include Header Files
#include "Setup.h"

//Include C-Files
#include "Pin_Setup.cpp"
#include "Clock_Setup.cpp"
#include "Timer_Setup.cpp"
#include "ADC_Setup.cpp"
#include "Systick.cpp"
#include "algorithms.cpp"
#include "display.h"

//Für Tests in Main
#include "Sensors.cpp"



//Library-Include
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
  HardwareSerial Serial1(BLUETOOTH_RX, BLUETOOTH_TX);
  Serial1.begin(115200);
  display_setup();


//Start Systick Timer
  timer14.resume();
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

  while(Distance_Sensor_Mid_MM < 2500){
    Distanz_Mid_Sensor();
  }
   while(Distance_Sensor_Mid_MM > 2500){
    Distanz_Mid_Sensor();
  }
  delay(1000);
   while(Distance_Sensor_Mid_MM < 2500){
    Distanz_Mid_Sensor();
    Forward(10);
  }
  
}

//put funktions here