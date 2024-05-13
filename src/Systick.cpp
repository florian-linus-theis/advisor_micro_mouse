
#include <iostream>
#include "Arduino.h"
#include "Sensoren.cpp"


HardwareTimer timer4(TIM4);
HardwareTimer timer3(TIM3);


class Timer_Interrupts{
 public:
  

  void Timer4_Setup(void){
  // Configure timer
    timer4.setPrescaleFactor(50); // Set prescaler to 50
    timer4.setOverflow(16);      // Set overflow to 16 = 50us Intervalle
    timer4.attachInterrupt(Sensor_Interrupt);
    timer4.refresh();
    timer4.resume();
    NVIC_EnableIRQ(TIM4_IRQn);
}

  void Systick_Setup(void){
    timer3.setPrescaleFactor(122000); // Set prescaler to 122 000
    timer3.setOverflow(65535);      // Set overflow to 65535 = 2 ms Intervalle
    timer3.attachInterrupt(Systick_Interrupt);  
    timer3.refresh();
    timer3.resume();
  }

  void Sensor_Interrupt() {
    Messung_Hell.push_back(analogRead(Channel_Sensoren[interrupt_counter])); //Array wird von hinten nach vorne durchlaufen,sodass beim pushback nicht die Reihenfolge verfälscht wird
    digitalWrite(Channel_Emitter[interrupt_counter], LOW);
    interrupt_counter++;
    if(interrupt_counter == 7){
      digitalWrite(Channel_Sensoren[interrupt_counter], HIGH);
    }
  }
  
  void Systick_Interrupt(){

    //Alle Steuerungsfunktionen, Messungen und sonstige Regelsystem-Funktionen werden hier untergebracht


    }

  void update(){
    //Motor, Sensoren und sonstiges wird hier geupdated

  }
};
