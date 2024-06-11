#include "Setup\Setup.h"

int encoder_right = 0;
int encoder_left = 0;
int encoder_right_last_time = 0;
int encoder_left_last_time = 0;
volatile double current_delta_speed_L = 0;
volatile double current_delta_speed_R = 0;
volatile double current_avg_speed = 0;
int current_micros = 0;
int last_micros = 0;
volatile double current_angle = 0;
std::vector<int> PID_values = {0, 0};

void update_encoders();
void print_encoders();
void print_sensors();
void print_pid();
void calc_speed();
void calc_angle();
int system_clock_micros();


void Systick_Interrupt() {
  if (!SETUP_COMPLETE) {return;}

  // first read encoder values 
  static int counter = 0; 
  update_encoders();
  
  // setting old values for comparison (every 5th iteration)
  if (counter % 5 == 0) {
    for(int i=0; i < 7; i++){
      Last_Distance_Sensor[i] = Distance_Sensor[i];
    }
  }
  // then read sensor values
  Distanz_Messung_Sensoren();
  // print_sensors(); // just for test purposes
  
  // lastly update the PID controller 
  // only determine PID case if not set manually
  if (!SET_PID_MANUALLY){
    CURRENT_CASE_PID = determine_PID_case();
  }
  PID_values = calc_correction(CURRENT_CASE_PID);
  PID_values_encoder = calc_correction(X_ERROR_ENCODER_BASED);
  PID_values = determine_correction_needed();
  calc_average_PID_values();
  counter++;
}


void update_encoders() {
  encoder_right = TIM5->CNT;
  encoder_left = TIM2->CNT;
  distance_traveled_L += encoder_left - encoder_left_last_time;
  distance_traveled_R += encoder_right - encoder_right_last_time;
  avg_distance_traveled = (distance_traveled_L + distance_traveled_R) / 2;
  calc_speed(); // calculate the speed of the robot before updating the last encoder values
  calc_angle(); // calcualte the angle of the robot
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


// calculate the speed of the robot
// speed = delta distance / delta time [mm / s]
// [mm / s], encoder resolution 592.65 [ticks / mm] --> 0.001687 = mm / tick
// 1.000.000 microsecs / sec
// Our systick interval is roughly at 11.65ms (is not a big issue if it is not exactly 11.65ms)
void calc_speed() {
  current_micros = system_clock_micros();
  int delta_time = current_micros - last_micros;
  // left wheel
  int delta_s = encoder_left - encoder_left_last_time;
  current_delta_speed_L = (static_cast<double>(delta_s) / delta_time ) * 1687.379; // [mm / s], encoder resolution 592.65 [ticks / mm], 0.001687 = mm / tick, -->  * 1000 ms / s
  // right wheel
  delta_s = encoder_right - encoder_right_last_time;
  current_delta_speed_R = (static_cast<double>(delta_s) / delta_time) * 1687.379; // [mm / s]
  current_avg_speed = (current_delta_speed_L + current_delta_speed_R) / 2; // [mm / s]
  last_micros = current_micros;
}

// Function to get the current microseconds
int system_clock_micros() {
    return DWT->CYCCNT / (SystemCoreClock / 1000000);
}

// 

void calc_angle(){
  int current_encoder_distance = distance_traveled_R - distance_traveled_L;
  // int encoder_sum = distance_traveled_L + distance_traveled_R;

  // current_distance = encoder_sum * MM_PER_TICK;
  current_angle = current_encoder_distance * DEGREE_PER_TICK;
}



