
//Header-Files Include
#include "Pin_Init.h"
#include "Systick.cpp"
#include "algorithms.cpp"

//Library-Include
#include <Arduino.h>

// put function declarations here:


void setup() {
  // put your setup code here, to run once:
  maze_setup(); // setting up the maze file
  Pin_Setup();
  Systick.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}

//put funktions here