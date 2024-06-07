#include "Setup\Setup.h"

int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS, IR_EMITTER_MID};
int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS, IR_SENSOR_MID};
int Blind_Sensor[7] = {0};
int Distance_Sensor[7] = {0};
bool SENSORS_CALIBRATED = false;
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

}

int air_values[7] = {0};
int maze_values[7] = {0};
int ballgrabber_values[7] = {0};

void calibration_average(int measurements_air, int measurements_maze);

void calibrate_sensors_dynamic(int measurements_air, int measurements_maze){
  digitalWrite(LED_GREEN, LOW);
  Buzzer_beep(3000, 3, 100);
  SENSORS_CALIBRATED = false; // we start from scratch again
  
    
  // Light LED and Wait for user finger
  display_print("Now in start square...", 1);
  digitalWrite(LED_GREEN, HIGH);
  start();

  Distanz_Messung_Sensoren();
  air_values[2] += Distance_Sensor[2];    //Left front
  air_values[5] += Distance_Sensor[5];    //Right front
  air_values[6] += Distance_Sensor[6];    //Mid front
  

  move_forward_middle_level(150, 0.45);   //move to the middle of cell
  rotate_left();

  //2 Messurements for Walls to average PID Errors in Move forward

  for (int i = 0; i<measurements_maze; i++){                        //Definition von Drehpunkt Kurve
    Distanz_Messung_Sensoren();
    maze_values[2] += Distance_Sensor[2];    //Left front
    maze_values[5] += Distance_Sensor[5];    //Right front
    maze_values[6] += Distance_Sensor[6];    //Mid front
  }

  turn_around_right();

  for (int i = 0; i<measurements_maze; i++){                        //Definition von Drehpunkt Kurve
    Distanz_Messung_Sensoren();
    maze_values[2] += Distance_Sensor[2];    //Left front
    maze_values[5] += Distance_Sensor[5];    //Right front
    maze_values[6] += Distance_Sensor[6];    //Mid front
  }

  Buzzer_beep(3000, 3, 100);

  BackwardLeft(150);
  BackwardRight(150);
  delay(500);                             //moves backward against wall
  stop();

    // measure in the maze and add up to array
  for (int i=0; i<measurements_maze; i++){
    Distanz_Messung_Sensoren();
    for(int j=3; j<5; j++) {                  //only right sensors will see wall
      maze_values[j] += Distance_Sensor[j];
      }
  }
  //air values left side
  for (int i=0; i<measurements_air; i++){
    Distanz_Messung_Sensoren();
    for(int j=0; j<2; j++) {                  //only left sensors will see air
      air_values[j] += Distance_Sensor[j];
    }
  }

  Buzzer_beep(3000, 3, 100);

  move_actual(150);
  delay(200);
  stop();
  turn_around_right();

  BackwardLeft(150);
  BackwardRight(150);
  delay(500);                           //moves againt opposit wall with back
  stop();

    // measure in the maze and add up to array
  for (int i=0; i<measurements_maze; i++){
    Distanz_Messung_Sensoren();
    for(int j=0; j<2; j++) {                  //only left sensors will see air
      air_values[j] += Distance_Sensor[j];
      }
  }
  //air values left side
  for (int i=0; i<measurements_air; i++){
    Distanz_Messung_Sensoren();
    for(int j=3; j<5; j++) {                  //only left sensors will see wall
      maze_values[j] += Distance_Sensor[j];
    }
  }

  Buzzer_beep(3000, 3, 100);

  move_actual(150);
  delay(200);
  stop();
  turn_around_right();

  BackwardLeft(150);
  BackwardRight(150);
  delay(500);                           //moves againt opposit wall with back
  stop();

  SENSORS_CALIBRATED = true;

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
  calibration_average(measurements_air, measurements_maze);
}

void calibration_average(int measurements_air, int measurements_maze){
  for(int i=0 ; i<2 ; i++){     //Left Sensors
        calibration_sensor[i] = static_cast<int>(std::round(static_cast<double>(air_values[i]) / measurements_air));
        NeutralSensorValues[i] = static_cast<int>(std::round(static_cast<double>(maze_values[i]) / measurements_maze));
        ballgrabber_calibration[i] = static_cast<int>(std::round(static_cast<double>(ballgrabber_values[i]) / measurements_maze));
  }
  
  calibration_sensor[2] = static_cast<int>(std::round(static_cast<double>(air_values[2]) /measurements_air));
  NeutralSensorValues[2] = static_cast<int>(std::round(static_cast<double>(maze_values[2]) / (measurements_maze*2)));     //dopple wall measurment, average PID errors and Offset
  ballgrabber_calibration[2] = static_cast<int>(std::round(static_cast<double>(ballgrabber_values[2]) / measurements_maze));


  for(int i=3 ; i<5 ; i++){     //Right sensors
        calibration_sensor[i] = static_cast<int>(std::round(static_cast<double>(air_values[i]) / measurements_air));
        NeutralSensorValues[i] = static_cast<int>(std::round(static_cast<double>(maze_values[i]) / measurements_maze));
        ballgrabber_calibration[i] = static_cast<int>(std::round(static_cast<double>(ballgrabber_values[i]) / measurements_maze));
  }

  for(int i=5 ; i<7 ; i++){     //Right sensors
        calibration_sensor[i] = static_cast<int>(std::round(static_cast<double>(air_values[i]) / measurements_air));
        NeutralSensorValues[i] = static_cast<int>(std::round(static_cast<double>(maze_values[i]) / (measurements_maze * 2)));
        ballgrabber_calibration[i] = static_cast<int>(std::round(static_cast<double>(ballgrabber_values[i]) / measurements_maze));
  }

}