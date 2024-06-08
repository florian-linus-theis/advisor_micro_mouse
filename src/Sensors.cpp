#include "Setup\Setup.h"

int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS, IR_EMITTER_MID};
int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS, IR_SENSOR_MID};
int Blind_Sensor[7] = {0};
int Distance_Sensor[7] = {0};
int Last_Distance_Sensor[7] = {0};
int calibration_sensor[7] = {0};
int MinSensorValues[7] = {0}; // for wall detection
int MaxSensorValues[7] = {0}; // for PID range mapping
bool SENSORS_CALIBRATED = false;

bool Walls_Flag[7] ={};

//Meassurement Data Vector
volatile int interrupt_counter = 0;

void Distanz_Messung_Hell(void);
void Distanz_Messung_Sensoren(void);


//Main Navigation Infrared Sensor Measurement - - - - - - - - - - - - - - - - - - - - - -
void Distanz_Messung_Sensoren(void){
  Distanz_Messung_Blind();
  Distanz_Messung_Hell();

  for(int i = 0; i < 7; i++){ 
      Walls_Flag[i] = Distance_Sensor[i] > 120;       //Platzhalter für Linearisierte Sensorwerte und Auswertung in MM
  }
}


void Distanz_Messung_Blind(void){
  for(int i = 0; i < 7; i++){
    Blind_Sensor[i] = analogRead(Channel_Sensoren[i]);
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
  // return if setup is not complete
  if (!SETUP_COMPLETE) {return;}
  
  if (SENSORS_CALIBRATED){
      Distance_Sensor[interrupt_counter] =  analogRead(Channel_Sensoren[interrupt_counter]) - Blind_Sensor[interrupt_counter] - calibration_sensor[interrupt_counter]; // Read Sensor Values with previous correction
  } else {
      Distance_Sensor[interrupt_counter] =  analogRead(Channel_Sensoren[interrupt_counter]) - Blind_Sensor[interrupt_counter]; // Read Sensor Values without previous correction
  }
  //10 is new level of calibrated sensors. 100 +. Negative Values are avoided

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
    ble->println(String(i) + ":" + String(Distance_Sensor[i] - 100));   //Userfriendly read
  }
}



// Function to calibrate the sensors - first in the air, then in the maze start cell.
void calibrate_sensors(int measurements_air, int measurements_maze){
    SENSORS_CALIBRATED = false; // we start from scratch again
    int air_values[7] = {0};
    int neutral_values[7] = {0};
    int min_values[7] = {0};
    int max_values[7] = {0};
    int ballgrabber_values[7] = {0};

    digitalWrite(LED_GREEN, LOW);
    Buzzer_beep(3000, 3, 100);


    // measure in the air and add up to array
    for (int i=0; i<measurements_air; i++){
        Distanz_Messung_Sensoren();
        for(int j=0; j<7; j++) {
            air_values[j] += Distance_Sensor[j];
        }
    }

    // calculate average by dividing through # of measurements
    // important to calculate the calibration values before other measurements
    for (int i=0; i<7; i++){
        calibration_sensor[i] = static_cast<int>(std::round(static_cast<double>(air_values[i]) / measurements_air));
    }

    SENSORS_CALIBRATED = true; // setting sensors calibrated to true so that for neutral values we use the right correction
    
    // Light LED and Wait for user finger
    display_print("Now in start square...", 1);
    digitalWrite(LED_GREEN, HIGH);
    start("right");

    // measure in the start cell of maze and add up to array
    for (int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        for(int j=0; j<7; j++) {
            neutral_values[j] += Distance_Sensor[j];
        }
    }

    // Min values for right wall & Max values for left wall
    display_print("Move to LEFT side", 1);
    digitalWrite(LED_GREEN, HIGH);
    wait_for_other_side("left"); // move left
    start("right"); // finger right
    for(int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        min_values[4] += Distance_Sensor[4];
        min_values[5] += Distance_Sensor[5];
        max_values[0] += Distance_Sensor[0];
        max_values[1] += Distance_Sensor[1];
    }

    // Min values for left wall & Max values for right wall
    display_print("Move to RIGHT side and wheels on edge", 1);
    digitalWrite(LED_GREEN, HIGH);
    wait_for_other_side("right"); // move right
    start("left"); // finger left
    for(int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        min_values[0] += Distance_Sensor[0];
        min_values[1] += Distance_Sensor[1];
        max_values[4] += Distance_Sensor[4];
        max_values[5] += Distance_Sensor[5];
    }

    // Now Min values front
    display_print("Move to CENTER BACK wheels on edge", 1);
    digitalWrite(LED_GREEN, HIGH);
    start("left");
    for(int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        min_values[2] += Distance_Sensor[2];
        min_values[3] += Distance_Sensor[3];
        min_values[6] += Distance_Sensor[6];
    }

    // for ballgrabber calibration while drivig towards ballgrabber
    display_print("Now facing Ballgrabber, finger left", 1);
    digitalWrite(LED_GREEN, HIGH);
    start("left");

    for(int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        for(int j=0; j<7; j++) {
            if (j == 4 || j == 5) continue;
            ballgrabber_values[j] += Distance_Sensor[j];
        }
    }

    // ballgrabber calibration while driving back from ballgrabber
    display_print("Now facing away from Ballgrabber, finger right", 1);
    digitalWrite(LED_GREEN, HIGH);
    start("right");
    for(int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        for(int j=0; j<7; j++) {
            if (j != 4 || j != 5) continue;
            ballgrabber_values[j] += Distance_Sensor[j];
        }
    }

    // calculate average by dividing through # of measurements
    for(int i=0 ; i<7 ; i++){
        NeutralSensorValues[i] = static_cast<int>(std::round(static_cast<double>(neutral_values[i]) / measurements_maze));
        ballgrabber_calibration[i] = static_cast<int>(std::round(static_cast<double>(ballgrabber_values[i]) / measurements_maze));
        MinSensorValues[i] = static_cast<int>(std::round(static_cast<double>(min_values[i]) / measurements_maze));
        MaxSensorValues[i] = static_cast<int>(std::round(static_cast<double>(max_values[i]) / measurements_maze));
    }

}