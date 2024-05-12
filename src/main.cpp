
//Header-Files Include
#include "Pin_Init.h"
#include "Systick.cpp"

//Library-Include
#include <Arduino.h>

// put function declarations here:


void setup() {
  // put your setup code here, to run once:
  Timer_Interrupts timer_interrupts;


  timer_interrupts.Systick_Setup();     //TIM3
  timer_interrupts.Timer4_Setup();      //TIM4

  Pin_Setup();
  
}

void loop() {
  // put your main code here, to run repeatedly:
}

//put funktions here