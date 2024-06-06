#include "Setup\Setup.h"

int encoder_right = 0;
int encoder_left = 0;
int encoder_right_last_time = 0;
int encoder_left_last_time = 0;
std::vector<int> PID_values = {0, 0};

void update_encoders();
void print_encoders();
void print_sensors();
void print_pid();


void Systick_Interrupt() {
  if (!SETUP_COMPLETE) {return;}

  // first read encoder values 
  // static int counter = 0; 
  update_encoders();
  
  // then read sensor values
  Distanz_Messung_Sensoren();
  // print_sensors(); // just for test purposes
  
  // lastly update the PID controller 
  // only determine PID case if not set manually
  if (!SET_PID_MANUALLY){
    CURRENT_CASE_PID = determine_PID_case();
  }
  PID_values = calc_correction(CURRENT_CASE_PID);
  // counter++; 
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

void print_sensors(){
  ble->println("S0: " + String(Distance_Sensor[0]) + "\n" +
                "S1: " + String(Distance_Sensor[1]) + "\n" +
                "S2: " + String(Distance_Sensor[2]) + "\n" +
                "S3: " + String(Distance_Sensor[3]) + "\n" +
                "S4: " + String(Distance_Sensor[4]) + "\n" +
                "S5: " + String(Distance_Sensor[5]) + "\n" +
                "S6: " + String(Distance_Sensor[6]) + "\n");
}

void print_pid(){
  ble->println("PID: " + String(PID_values[0]) + " " + String(PID_values[1]));
}


