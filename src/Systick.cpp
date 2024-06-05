#include "Setup\Setup.h"

int encoder_right = 0;
int encoder_left = 0;
int encoder_right_last_time = 0;
int encoder_left_last_time = 0;

void update_encoders();
void print_encoders();


void Systick_Interrupt() {
  if (!SETUP_COMPLETE) {
    return;
  }
  // first read encoder values 
  static int counter = 0; 
  update_encoders();
  if (counter % 10 == 0) {
    print_encoders(); // print_encoders(); just for test purposes
  }
  
  // then read sensor values
  Distanz_Messung_Sensoren();
  
  // lastly update the PID controller 
  

  counter++; 
}


void update_encoders() {
  encoder_right = TIM5->CNT;
  encoder_left = TIM2->CNT;
  distance_traveled_L += encoder_left - encoder_left_last_time;
  distance_traveled_R += encoder_right - encoder_right_last_time;
  avg_distance_traveled = (distance_traveled_L + distance_traveled_R) / 2;
  encoder_left_last_time = encoder_left;
  encoder_right_last_time = encoder_right;
}

void reset_encoders() {
  // Reset the encoder counters in the hardware registers
  TIM5->CNT = 0;
  TIM2->CNT = 0;

  // Reset all distance and average distance tracking variables
  distance_traveled_L = 0;
  distance_traveled_R = 0;
  avg_distance_traveled = 0;

  // Initialize last encoder readings to zero
  encoder_right_last_time = 0;
  encoder_left_last_time = 0;
}


void print_encoders() {
  // cocatenate that as one string and print it to the serial monitor
    ble->println("Current Duty Cycle: " + String(current_duty_cycle) + "\n" +
           " Avg distance travelled up to this point: " + String(avg_distance_traveled) + "\n" +
           " Current Time in millis: " + String(millis()) + "\n");
}


