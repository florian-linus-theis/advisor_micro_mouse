//Include Header Files
//#include "Setup.h"

//Include C-Files
#include "Pin_Setup.cpp"
#include "Clock_Setup.cpp"
#include "Timer_Setup.cpp"
#include "ADC_Setup.cpp"
#include "Systick.cpp"
#include "algorithms.cpp"


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


//Start Systick Timer
  timer14.resume();
}


void loop() {
  // put your main code here, to run repeatedly:
}

//put funktions here