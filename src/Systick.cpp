#include "Setup\Setup.h"

int encoder_right = 0;
int encoder_left = 0;

void update_encoders();
void print_encoders();


void Systick_Interrupt() {
  // first read encoder values 
  static int counter = 0; 
  update_encoders();
  // if (counter % 100 == 0) {
  //   print_encoders(); // print_encoders(); just for test purposes
  // }
  // print_encoders();
  
  // then read sensor values
  Distanz_Messung_Sensoren();
  
  // lastly update the PID controller 


  counter++; 
}


void update_encoders() {
  static int encoder_right_last_time = 0;
  static int encoder_left_last_time = 0;
  encoder_right = TIM5->CNT;
  encoder_left = TIM2->CNT;
  distance_traveled_L += encoder_left - abs(encoder_left_last_time);
  distance_traveled_R += encoder_right - abs(encoder_right_last_time);
  avg_distance_traveled = (distance_traveled_L + distance_traveled_R) / 2;
  encoder_left_last_time = encoder_left;
  encoder_right_last_time = encoder_right;
}

void print_encoders() {
  ble->print("Encoder Right: ");
  ble->println(encoder_right);
  ble->print("Encoder Left: ");
  ble->println(encoder_left);
  ble->print("Distance Traveled Left: ");
  ble->println(distance_traveled_L);
  ble->print("Distance Traveled Right: ");
  ble->println(distance_traveled_R);
  ble->print("Average Distance Traveled: ");
  ble->println(avg_distance_traveled);
  ble->println(); // empty line
}


