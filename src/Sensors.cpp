#include "Setup.h"

int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS};
int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS};

//Meassurement Data Vector
std::vector<uint32_t> Messung_Blind;
std::vector<uint32_t> Messung_Hell;
uint32_t Distance_Sensor_Mid_MM;

int interrupt_counter;
int Flag_Mid;


//Main Navigation Infrared Sensor Measurement - - - - - - - - - - - - - - - - - - - - - -

void Distanz_Messung_Blind(void){
  for(int i = 0; i < 6; i++){
    Messung_Blind.push_back(analogRead(Channel_Emitter[i]));
  }
}


void Distanz_Messung_Hell(void) {
  interrupt_counter = 0;    // Reset the interrupt counter
  Messung_Hell.clear(); // Clear previous measurements

  // Turn on the first emitter
  digitalWrite(Channel_Emitter[0], HIGH);

  // Enable the interrupt to start the measurement process
  timer6->setCount(0);
  timer6->refresh();
  timer6->resume();

  // Wait for the process to complete
  while (interrupt_counter < 6) {} // Wait in a non-blocking way (e.g., other code can run here)
}

void Timer6_Interrupt(void) {
  Messung_Hell.push_back(analogRead(Channel_Sensoren[interrupt_counter]));  // Read the current sensor value 
  digitalWrite(Channel_Emitter[interrupt_counter], LOW);                        // Turn off the current emitter

  interrupt_counter++;  // Move to the next sensor

  if (interrupt_counter < 6) {      	  // Turn on the next emitter
    digitalWrite(Channel_Emitter[interrupt_counter], HIGH);
  } else if (interrupt_counter >= 6){   // All emitters processed
    digitalWrite(Channel_Emitter[interrupt_counter], LOW);
    timer6->pause();
  }
}



//Mid Infrared Sensor Measurement - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Distanz_Mid_Sensor(void){
    Flag_Mid = 0;
    digitalWrite(IR_EMITTER_MID, HIGH);
    timer7->setCount(0);
    timer7->refresh();
    timer7->resume();
    
    while(Flag_Mid == 0){}
}

void Timer7_Interrupt(void){
  digitalWrite(LED_BLUE, LOW);
    Distance_Sensor_Mid_MM = analogRead(IR_SENSOR_MID);
    digitalWrite(IR_EMITTER_MID, LOW);
    Flag_Mid++;
    timer7->pause();
}



// Print Measured Sensor Values to Bluetooth Module - - - - - - - - - - -
void printDistanzSensoren(void) {
  ble->println("Messung_Hell Messwerte:");
  for (int i = 0; i < 6; i++) {
    ble->print("Sensor ");
    ble->print(i);
    ble->print(": ");
    ble->println(Messung_Hell[i]);
  }
  ble->print("Distanz_Sensor Mitte:");
  ble->println(Distance_Sensor_Mid_MM);
}