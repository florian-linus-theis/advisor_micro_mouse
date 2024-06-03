//Include Librarys
#include "Arduino.h"
#include "cmath"
#include "vector"
#include "tuple"
#include "string"
#include "HardwareTimer.h"
#include "HardwareSerial.h"
#include "iostream"
#include "wiring.h"
#include "Wire.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "./location.h"
//#include "Sensors.h"



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
#define DUTY_SLOW_ROTATION 50
#define DUTY_FAST 400
#define DUTY_FAST_CURVE 200
#define MINIMUM_DUTY 50
#define TICKS_INNER_WHEEL 8600 * 4
#define TICKS_OUTER_WHEEL 18105 * 4
#define DISTANCE_DUTY_MIN_TO_ZERO 10000 // bit less than half braking distance -> approx 2cm
#define KNOWN_BRAKE_DIST_AT_DUTY_SLOW 24000 // ukmars has 27mm braking distance at their exploration speed, assuming we have 40mm braking distance -> 24000 ticks (rounded at 600 ticks per mm) wanna over estimate that
#define SPEED_TO_DUTY_FACTOR 3 // TODO: adjust this



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



//Clock_Setup
extern void Clock_Setup(void);



//Timer_Setup
extern HardwareTimer *timer14;
extern HardwareTimer *timer3;
extern HardwareTimer *timer4;
extern HardwareTimer *timer2;
extern HardwareTimer *timer5;
extern HardwareTimer *timer6;
extern HardwareTimer *timer10;
extern HardwareTimer *timer1;

extern void Timer_Setup(void);
extern void Timer4_Setup_Motor(void); // needed for manual resetting of the motor timers after ballgrabbing
extern void Timer4_Setup_Servo(void); // needed for manual setting of the servo timer after driving to the ball
extern void set_Interrupt_Priority(void);



//Systick
extern void Systick_Interrupt(void);
extern int Systick_Counter;
// Global variables updated during systick 
extern int encoder_right_total; 
extern int encoder_left_total;
extern int distance_traveled_L; // TODO: evtl float
extern int distance_traveled_R; // TODO: evtl float
extern int avg_distance_traveled; // TODO: evtl float
extern int current_duty_cycle;
extern int duty_L;
extern int duty_R;
extern int current_speed; 


// Drive Motors
extern void ForwardLeft(int);
extern void ForwardRight(int);
extern void BackwardLeft(int);
extern void BackwardRight(int);
extern void move_forward_middle_level(int, float);
extern void stop(); 
extern void accelerate();
extern void decelerate();
extern void left_curve(int);
extern void right_curve(int);
extern void move_actual(int);
extern void move_forward_different(int, int, float);
extern void accelerate_different(int, int);
extern void decelerate_different(int, int);


//ADC_Setup
extern void ADC_Setup(void);




//Move to new Header File - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Sensors
extern void Timer6_Interrupt(void);

extern void Distanz_Messung_Sensoren(void);
extern void Distanz_Messung_Blind(void);
extern void Distanz_Messung_Hell(void);
extern void Distanz_Mid_Sensor(void);
extern void printDistanzSensoren(void);

extern int Channel_Emitter[];
extern int Channel_Sensoren[];
extern int Distance_Sensor[];
extern int calibration_sensor[];

extern int Distance_Sensor_Mid_MM;
extern double Abs_Sensor_Calibration;

extern int interrupt_counter;
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
extern bool encoderTurned;
extern bool confirmationPending;

extern void Buzzer_beep(int, int);