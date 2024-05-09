
#include <iostream>
#include "Arduino.h"
#include "Sensoren.cpp"
#include "Systick.cpp"

class Systick {
 public:

  void Timer4_Setup(void){
  // Configure timer
    timer4.setPrescaleFactor(50); // Set prescaler to 50
    timer4.setOverflow(16);      // Set overflow to 16 = 50us Intervalle
    timer4.attachInterrupt(OnTimer4Interrupt);
    timer4.refresh();
}

  void OnTimer4Interrupt() {
    Messung_Hell.push_back(analogRead(Channel_Sensoren[6 - interrupt_counter])); //Array wird von hinten nach vorne durchlaufen,sodass beim pushback nicht die Reihenfolge verfälscht wird
    digitalWrite(Channel_Emitter[6 - interrupt_counter], LOW);
    interrupt_counter++;
    if(interrupt_counter =! 7){
      continue;
    }
    else{
    digitalWrite(Channel_Sensoren[6 - interrupt_counter], HIGH);
    }


  void update() {
    

  }
};
