#include "Setup\Setup.h"

int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS, IR_EMITTER_MID};
int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS, IR_SENSOR_MID};
int Distance_Sensor[7] = {};
int calibration_sensor[7] = {370,374,583,247,630,372,0};
//int calibration_sensor[7] = {137,221,168,175,241,182,337};    //Kalibierung fehlt
//int calibration_sensor[7] = {370,374,583,247,630,372,0};
bool Walls_Flag[7] ={};

//Meassurement Data Vector

int interrupt_counter;
volatile int Messuring1;

void Distanz_Messung_Hell(void);
void Distanz_Messung_Sensoren(void);


//Main Navigation Infrared Sensor Measurement - - - - - - - - - - - - - - - - - - - - - -

void Distanz_Messung_Blind(void){
  for(int i = 0; i < 7; i++){
    Messuring1 = analogRead(Channel_Sensoren[i]);           //second Messureing should increase accuracy of sensorreadings
    Distance_Sensor[i] = (Messuring1 + analogRead(Channel_Sensoren[i])) % 2;
  }
}

void Distanz_Messung_Sensoren(void){
  Distanz_Messung_Blind();
  Distanz_Messung_Hell();

  for(int i = 0; i < 7; i++){
    if (Distance_Sensor[i] > 20){     //Avoids unprecies informations of walls in lay
      Walls_Flag[i] = true;       //Platzhalter für Linearisierte Sensorwerte und Auswertung in MM
    }
    else{
      Walls_Flag[i] = false;
    }
  }
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
  while (interrupt_counter < 7) {} // Wait in a non-blocking way (e.g., other code can run here)
}

void Timer6_Interrupt(void) { 
  Distance_Sensor[interrupt_counter] =  analogRead(Channel_Sensoren[interrupt_counter]) - Distance_Sensor[interrupt_counter] - calibration_sensor[interrupt_counter]; // Read Sensor Values
  if(Distance_Sensor[interrupt_counter] < 0){
    Distance_Sensor[interrupt_counter] = 0;         //Allow only positive Values
  }
  digitalWrite(Channel_Emitter[interrupt_counter], LOW);                        // Turn off the current emitter

  interrupt_counter++;  // Move to the next sensor

  if (interrupt_counter < 7) {      	  // Turn on the next emitter
    digitalWrite(Channel_Emitter[interrupt_counter], HIGH);
  } else if (interrupt_counter >= 7){   // All emitters processed
    timer6->pause();
  }
}


// Print Measured Sensor Values to Bluetooth Module - - - - - - - - - - -
void printDistanzSensoren(void) {
  for (int i = 0; i < 7; i++) {
    ble->print(i);
    ble->print(": ");
    ble->println(Distance_Sensor[i]);
  }
}