#include "Setup\Setup.h"

int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS, IR_EMITTER_MID};
int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS, IR_SENSOR_MID};
int Blind_Sensor[7] = {0};
int Distance_Sensor[7] = {0};
bool SENSORS_CALIBRATED = false;
int max_value_left = 0, max_value_right = 0;
// int calibration_sensor[7] = {370,374,583,247,630,372,0};
//int calibration_sensor[7] = {137,221,168,175,241,182,337};    //Kalibierung fehlt
//int calibration_sensor[7] = {370,374,583,247,630,372,0};
int calibration_sensor[7] = {320, 516, 464, 200, 630, 348, 709};

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
    int maze_values[7] = {0};
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

    // 
    SENSORS_CALIBRATED = true; // setting sensors calibrated to true so that for neutral values we use the right correction
    
    // Light LED and Wait for user finger
    display_print("Now in start square...", 1);
    digitalWrite(LED_GREEN, HIGH);
    start();

    // measure in the maze and add up to array
    for (int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        for(int j=0; j<7; j++) {
            maze_values[j] += Distance_Sensor[j];
        }
    }
    display_print("Now facing Ballgrabber", 1);
    digitalWrite(LED_GREEN, HIGH);
    start();

    for(int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        for(int j=0; j<7; j++) {
            if (j == 4 || j == 5) continue;
            ballgrabber_values[j] += Distance_Sensor[j];
        }
    }

    display_print("Now facing away from Ballgrabber", 1);
    digitalWrite(LED_GREEN, HIGH);
    start();
    for(int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        for(int j=0; j<7; j++) {
            if (j != 4 || j != 5) continue;
            ballgrabber_values[j] += Distance_Sensor[j];
        }
    }

    // calculate average by dividing through # of measurements
    for(int i=0 ; i<7 ; i++){
        calibration_sensor[i] = static_cast<int>(std::round(static_cast<double>(air_values[i]) / measurements_air));
        NeutralSensorValues[i] = static_cast<int>(std::round(static_cast<double>(maze_values[i]) / measurements_maze));
        ballgrabber_calibration[i] = static_cast<int>(std::round(static_cast<double>(ballgrabber_values[i]) / measurements_maze));
    }

    display_print("Now close to left wall", 1);
    digitalWrite(LED_GREEN, HIGH);
    start();
    for(int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        max_value_left += calcError(1);
    }
    ble->println(max_value_left);
    max_value_left = max_value_left / measurements_maze;
    ble->println("max_value_left: " + String(max_value_left));


    display_print("Now close to right wall", 1);
    digitalWrite(LED_GREEN, HIGH);
    start();

    for(int i=0; i<measurements_maze; i++){
        Distanz_Messung_Sensoren();
        max_value_right += calcError(1);
    }
    max_value_right = max_value_right / measurements_maze;
    ble->println("max_value_right: " + String(max_value_right));

    

}