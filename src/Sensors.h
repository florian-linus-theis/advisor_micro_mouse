#pragma once
#include "Setup\Setup.h"

int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS, IR_EMITTER_MID};
int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS, IR_SENSOR_MID};
int Distance_Sensor[7] = {};
int calibration_sensor[7] = {165,185,181 ,174,238,161, 0};    //Kalibierung fehlt
bool Walls_Flag[7] ={};

//Meassurement Data Vector
double Abs_Sensor_Calibration;

int interrupt_counter;

void Distanz_Messung_Hell(void);
void Distanz_Messung_Sensoren(void);


//Main Navigation Infrared Sensor Measurement - - - - - - - - - - - - - - - - - - - - - -

void Distanz_Messung_Blind(void){
  for(int i = 0; i < 7; i++){
    Distance_Sensor[i] = analogRead(Channel_Sensoren[i]);
  }
}

void Distanz_Messung_Sensoren(void){
  Distanz_Messung_Blind();
  Distanz_Messung_Hell();

  for(int i = 0; i < 7; i++){
    if (Distance_Sensor[i] > 0){
      Walls_Flag[i] = false;       //Platzhalter für Linearisierte Sensorwerte und Auswertung in MM
    }
    else{
      Walls_Flag[i] = true;
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
    digitalWrite(Channel_Emitter[interrupt_counter], LOW);
    timer6->pause();
  }
}


// Print Measured Sensor Values to Bluetooth Module - - - - - - - - - - -
void printDistanzSensoren(void) {
  ble->println("Messung_Hell Messwerte:");
  for (int i = 0; i < 7; i++) {
    ble->print("Sensor ");
    ble->print(i);
    ble->print(": ");
    ble->println(Distance_Sensor[i]);
  }
}