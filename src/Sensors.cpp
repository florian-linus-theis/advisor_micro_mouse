#include "Setup.h"

int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS};
int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS};

//Meassurement Data Vector
std::vector<uint32_t> Messung_Blind;
std::vector<uint32_t> Messung_Hell;
uint32_t Distance_Sensor_Mid_MM;

int interrupt_counter;
int Flag_Mid;  

HardwareSerial Serial1(BLUETOOTH_RX, BLUETOOTH_TX);


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
  Timer6_Restart();

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
    Timer6_Pause();
  }
}



//Mid Infrared Sensor Measurement - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Distanz_Mid_Sensor(void){
    Flag_Mid = 0;
    digitalWrite(IR_EMITTER_MID, HIGH);
    Timer7_Restart();
    
    while(Flag_Mid == 0){}
}

void Timer7_Interrupt(void){
    Distance_Sensor_Mid_MM = analogRead(IR_SENSOR_MID);
    digitalWrite(IR_EMITTER_MID, LOW);
    Flag_Mid++;
    Timer7_Pause();
}



// Print Measured Sensor Values to Bluetooth Module - - - - - - - - - - -

void printDistanzSensoren(void) {
  Serial1.println("Messung_Hell Messwerte:");
  for (int i = 0; i < 6; i++) {
    Serial1.print("Sensor ");
    Serial1.print(i);
    Serial1.print(": ");
    Serial1.println(Messung_Hell[i]);
  }
  
  Serial1.print("Distanz_Sensor Mitte:");
  Serial1.println(Distance_Sensor_Mid_MM);
}