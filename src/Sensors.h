#include "Setup\Setup.h"

int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS};
int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS};
int Distance_Sensor[6] = {};
int calibration_sensor[6] = {165,185,181 ,174,238,161};

//Meassurement Data Vector
int Distance_Sensor_Mid_MM;
double Abs_Sensor_Calibration;

int interrupt_counter;
int Flag_Mid;

void Distanz_Messung_Hell(void);
void Distanz_Messung_Sensoren(void);
void Distanz_Mid_Sensor(void);


//Main Navigation Infrared Sensor Measurement - - - - - - - - - - - - - - - - - - - - - -

void Distanz_Messung_Blind(void){
  for(int i = 0; i < 6; i++){
    Distance_Sensor[i] = analogRead(Channel_Sensoren[i]);
  }
}

void Distanz_Messung_Sensoren(void){
  Distanz_Messung_Blind();
  Distanz_Messung_Hell();
  delay(1000);
}


void Distanz_Messung_Hell(void) {
  interrupt_counter = 0;    // Reset the interrupt counter

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
  Distance_Sensor[interrupt_counter] =  analogRead(Channel_Sensoren[interrupt_counter]) - Distance_Sensor[interrupt_counter] - calibration_sensor[interrupt_counter]; // Read Sensor Values
  if(Distance_Sensor[interrupt_counter] < 0){
    Distance_Sensor[interrupt_counter] = 0;         //Allow only positive Values
  }
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
    ble->println(Distance_Sensor[i]);
  }
  ble->print("Distanz_Sensor Mitte:");
  ble->println(Distance_Sensor_Mid_MM);
}