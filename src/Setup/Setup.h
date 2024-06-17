//Include Librarys
#pragma once //<-- Macht das hier faxen?
#include <Arduino.h>
#include "cmath"
#include "vector"
#include "tuple"
#include "string"
#include "iostream"
#include <wiring.h>
#include <Wire.h>
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "./location.h"
#include <stm32f4xx_hal.h> //necessary for Flash Memory Usage


//Pin Naming
//System
#define POWER_ENABLE    PC10  //Main Power Latch - enable directly after startup to keep Robot ON
#define V_BAT           PC0   //Battery Voltage divided by 1,47
#define SWD_IO          PA13  //Serial Wire Debug Pins
#define SWD_CLK         PA14

//Drive Motors & Encoders
#define MOTOR_ENABLE          PC11   //HIGH = Motor Driver disabled,   LOW = Motor Driver enabled !!!
#define MOTOR_RIGHT_PWM_1      PB7    //PWM Outputs to Motor H-Bridge
#define MOTOR_RIGHT_PWM_2      PB6
#define MOTOR_LEFT_PWM_1       PB4
#define MOTOR_LEFT_PWM_2       PB5
#define MOTOR_ENCODER_RIGHT_A  PA0    //Quadrature Motor Encoder Inputs
#define MOTOR_ENCODER_RIGHT_B  PA1
#define MOTOR_ENCODER_LEFT_A   PA15
#define MOTOR_ENCODER_LEFT_B   PB3 
#define MOTOR_nFAULT           PD2    //LOW if Motor Driver Fault Condition (Undervoltage, Overcurrent, Overtemperature)

//Servos
#define SERVO_ENABLE    PH1   //Servo Power Switch, disable to save energy when Servo not in use - HIGH = Power ON,  LOW = Power OFF
#define SERVO_PWM_1     PB8
#define SERVO_PWM_2     PB9

//Infrared Emmitters
#define IR_EMITTER_RF   PC8
#define IR_EMITTER_LF   PC9
#define IR_EMITTER_RD   PC6
#define IR_EMITTER_LD   PC7
#define IR_EMITTER_RS   PB14
#define IR_EMITTER_LS   PB15
#define IR_EMITTER_MID  PB13

//Infrared Sensors
#define IR_SENSOR_RF    PC2
#define IR_SENSOR_LF    PB1
#define IR_SENSOR_RD    PA2
#define IR_SENSOR_LD    PB0
#define IR_SENSOR_RS    PA3
#define IR_SENSOR_LS    PC5
#define IR_SENSOR_MID   PC4

//IMU
#define IMU_SCL         PA5   //SPI#1 @ 1MHz
#define IMU_MOSI        PA7
#define IMU_MISO        PA6
#define IMU_nCS         PA4
#define IMU_INT         PA8   //IMU Interrupt, currently not in use
#define IMU_FSYNC       PB12  //Frame Synchronisation Output, probably not neccesary?!

//OLED Display
#define OLED_SCL        PB10  //SSD1306 Display Driver over I2C#2, Adress: 0x3C
#define OLED_SDA        PB11
#define OLED_RESET      -1    // Reset pin # (or -1 if sharing Arduino reset pin)

//Bluetooth Module
#define BLUETOOTH_TX    PA9   //HM-19 Bluetooth Module over UART#1, Baud Rate: 115200
#define BLUETOOTH_RX    PA10

//User-Interface-Encoder
#define UI_BUTTON       PC12  //User Interface Push Button, normally HIGH
#define UI_ENCODER_A    PH0   //Quadrature Encoder Signals
#define UI_ENCODER_B    PC15

//Buzzer
#define BUZZER          PA11

//Debug RGB LED
#define LED_RED         PC1   //HIGH = LED OFF,   LOW = LED ON !!!
#define LED_GREEN       PA12
#define LED_BLUE        PC3



// ---------------------------------------
// Driving constants 
#define DUTY_SLOW 100
#define DUTY_SLOW_ROTATION 100 // --> Tested - works with 24000 ticks for rotation
#define DUTY_FAST 400
#define DUTY_FAST_CURVE 100
#define SPEED_MAPPING 365
#define SPEED_FAST 500
#define MINIMUM_DUTY 50
#define TICKS_INNER_WHEEL 31000 // thorugh testing
#define TICKS_OUTER_WHEEL 62000 // through testing
#define DISTANCE_DUTY_MIN_TO_ZERO 10000 // bit less than half braking distance -> approx 2cm
#define KNOWN_BRAKE_DIST_AT_DUTY_SLOW 24000 // ukmars has 27mm braking distance at their exploration speed, assuming we have 40mm braking distance -> 24000 ticks (rounded at 600 ticks per mm) wanna over estimate that
#define DUTY_TO_SPEED_FACTOR 0.25 // Duty 100 -> Speed Reached 350mm/s; Duty 150 -> Speed Reached 600 mm/s; Duty 200 -> Speed Reached 800 mm/s
#define SPEED_SLOW 365 // mm/s
#define KNOWN_BRAKE_DIST_AT_SPEED_SLOW 20615 // actual: 20615 ticks but rounded to 21000
#define MM_PER_TICK 0.000843628 // mm per tick during curves (already adding mean of both wheels) -> actual value: 0.0016873
#define DEGREE_PER_TICK 0.0016112  // degree per tick during curves (= 360 * MM_PER_TICK / (PI * 60))

// Global Variables 
extern bool SETUP_COMPLETE;

// Algorithms
extern void dfs_mapping();
extern void bfs_algorithm();
extern void a_star_algorithm();
extern bool MAPPING_COMPLETE; 

// ---------------------------------------
// PID 
extern std::vector<int> NeutralSensorValues;
extern std::vector<int> PID_constants; // Global variable to store the PID constants
extern std::vector<int> PID_values;
extern std::vector<int> PID_values_encoder;
extern std::vector<int> calc_correction(int);
extern std::vector<int> determine_correction_needed();
extern void reset_PID_values();
extern void reset_encoder_PID_values();
extern int determine_PID_case();
extern std::array<bool, 4> find_walls_forward_looking();
extern void pid_move_function(int);
extern void calc_average_PID_values();
extern void recalibrate_front_wall();
extern bool front_wall_detected();
extern bool side_walls_disappearing();
extern double differential;
extern double integral;
extern double proportional; 
extern std::vector<int> previous;
extern int CURRENT_CASE_PID; 
extern bool SET_PID_MANUALLY;
extern std::vector<int> remapped_error;
extern int calcError(int);
extern std::vector<int> max_values_left;
extern std::vector<int> max_values_right;
extern std::vector<int> max_values_back;
extern std::vector<int> max_values_front;
extern std::vector<int> max_values_lower_boundary;
extern std::vector<int> max_values_upper_boundary;
extern std::vector<int> calc_max_occuring_Errors();
extern bool PID_ENABLED;
extern void enable_PID();
extern void disable_PID();
extern std::vector<int> correction_offset;
extern double give_percent(double, int, int);
extern std::vector<double> previous_speed;
extern int calc_correction_speed(int, int, double, double, double);

enum PID_CASES{
    X_ERROR = 0,
    X_ERROR_LEFT_WALL_ONLY = 1,
    X_ERROR_RIGHT_WALL_ONLY = 2,
    X_ERROR_ENCODER_BASED = 3,
    Y_ERROR = 4,
    TRANSITION = 5,
    BLIND = 6,
    V_ERROR_BASE_SPEED = 7,
    V_ERROR_EQUAL_SPEED = 8,
    ENUM_END = 9
};

enum SPEED_CASES{
    LEFT_SPEED = 0,
    RIGHT_SPEED = 1,
    BOTH_SPEEDS = 2
};

// ---------------------------------------
// Algorithm constants
// Initializing maze size with constexpr to set them up as compile-time-constants rather than runtime constants
#define MAZE_WIDTH 16
#define MAZE_HEIGHT 16
extern std::vector<std::vector<Location>> maze; // Global variable to store the maze
extern bool BALLGREIFER; // Control Variable to check if the ballgreifer is present
extern std::vector<int> POSSIBLE_GOAL_POS_ONE; 
extern std::vector<int> POSSIBLE_GOAL_POS_TWO;
extern std::vector<int> POSSIBLE_GOAL_POS_THREE;
extern std::vector<int> POSSIBLE_GOAL_POS_FOUR;


//Object, Function and Variable Declaration - - - - - - - - - - - - - - - - - - - - -
//Pin_Setup
extern SPIClass *imu;
extern Adafruit_SSD1306 *display;
extern HardwareSerial *ble;

extern void Pin_Setup(void);
extern void Set_Output(void);


//ADC_Setup
extern void ADC_Setup(void);


//Timer_Setup
extern HardwareTimer *timer14;
extern HardwareTimer *timer3;
extern HardwareTimer *timer4;
extern HardwareTimer *timer2;
extern HardwareTimer *timer5;
extern HardwareTimer *timer6;
extern HardwareTimer *timer10;
extern HardwareTimer *timer1;
extern HardwareTimer *timer7;

extern void Timer_Setup(void);
extern void set_Interrupt_Priority(void);



//Systick
extern void Systick_Interrupt(void);
extern int Systick_Counter;
// Global variables updated during systick 
extern int encoder_right_total; 
extern int encoder_left_total;
extern volatile int distance_traveled_L; // TODO: evtl float
extern volatile int distance_traveled_L_PID; // TODO: evtl float
extern volatile int distance_traveled_R; // TODO: evtl float
extern volatile int distance_traveled_R_PID; // TODO: evtl float
extern volatile int avg_distance_traveled; // TODO: evtl float
extern int current_duty_cycle;
extern int duty_L;
extern int duty_R;
extern volatile double current_delta_speed_L;
extern volatile double current_delta_speed_R;
extern volatile double current_avg_speed;
extern volatile double current_angle; 
extern volatile double current_acc_R;
extern volatile double current_acc_L;
extern void reset_distance_traveled_after_straight(void);
extern void reset_distance_traveled_after_curve(void);
extern void reset_encoders(void);
extern int system_clock_micros();



// Driving Functions
extern void ForwardLeft(int);
extern void ForwardRight(int);
extern void BackwardLeft(int);
extern void BackwardRight(int);
extern void ForwardBoth(int);
extern void BackwardBoth(int);
extern void rotate_left();
extern void rotate_right();
extern void turn_around_right();
extern void turn_around_left();
extern void move_forward_middle_level(int, float);
extern void stop(); 
extern void accelerate();
extern void decelerate();
extern void left_curve(int);
extern void right_curve(int);
extern void curve_right();
extern void curve_left();
extern void move_actual();
extern void move_forward_different(int, int, float);
extern void accelerate_different(int, int);
extern int decelerate_different(int, int);
extern void drive_forward(int, int, float);
extern void backup_to_wall();
extern int speedToDutyCycle(double);


// Middle Layer Drving Functions
extern void grab_ball();





//Move to new Header File - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Sensors
extern void Timer6_Interrupt(void);

extern void Distanz_Messung_Sensoren(void);
extern void Distanz_Messung_Blind(void);
extern void Distanz_Messung_Hell(void);
extern void Distanz_Mid_Sensor(void);
extern void printDistanzSensoren(void);
extern void calibrate_sensors(int, int);
extern bool SENSOR_CALIBRATED;

extern int Channel_Emitter[];
extern int Channel_Sensoren[];
extern int Distance_Sensor[];
extern int Last_Distance_Sensor[];
extern int calibration_sensor[];
extern int MinSensorValues[];
extern int MaxSensorValues[];
extern int Distance_Sensor_Mid_MM;
extern double Abs_Sensor_Calibration;

extern volatile int interrupt_counter;
extern int Flag_Mid;



//User-Interface (display.cpp)
extern void handleModeSelection(void);
extern void updateEncoderState(void);
extern void display_print(std::string, int);
extern void handleEncoderTurn(void);
extern void handleEncoderButton(void);
extern int current_option;
extern int selected_option;
extern bool optionSelected;
extern volatile bool encoderTurned;
extern bool confirmationPending;

// Define an enum for all modes
enum Mode {
    MODE_STANDBY,
    MODE_SOFT_RESET,
    MODE_SHOW_DATA,
    MODE_MAP_MAZE,
    MODE_BFS,
    MODE_ASTAR,
    MODE_LOAD_FLASH,
    MODE_MAX, // This can be used to determine the number of modes
};


// Buzzer
extern void Buzzer_beep(int, int);
extern void Buzzer_beep(int, int, int);
extern void Buzzer_beep_noBlock(int, int, int);
extern void Timer7_Interrupt();

// Before Start
extern void calibrate_sensors(int, int);
extern void start(int);
extern void wait_for_other_side(std::string);

// Battery
extern void getBattery();
extern void drawBatteryStatus();
extern void print_Battery_Status();
// extern float battery_volts;  
// extern int battery_percentage; 


// Ballgrabber
extern std::vector<int> ballgrabber_calibration;
extern void grab_ball();


// MMS Interaction
extern std::vector<int> cur_position;
extern int cur_direction;
extern void move_forward_mapping(int);	

// softreset
extern void soft_reset();

//Flash Memory
#define FLASH_SECTOR_11_START_ADDR 0x080E0000
#define FLASH_SECTOR_11_END_ADDR 0x080FFFFF

extern std::vector<int8_t> buffer;
extern void serialize_maze(const std::vector<std::vector<Location>>&, std::vector<uint8_t>&);
extern void deserialize_maze(const std::vector<int8_t>&, std::vector<std::vector<Location>>&);   
extern void write_data_to_flash(uint32_t, const std::vector<uint8_t>&); 
extern void read_maze_from_flash(uint32_t, std::vector<int8_t>&);

// Music and celebration
extern void Gas_song();
extern void Gas_song_short();
extern void celebrate();
extern void move_ballgrabber_forward();
extern void move_ballgrabber_backward();