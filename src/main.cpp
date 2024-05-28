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
  // put your main code here, to run repeatedly:
}

//put funktions here