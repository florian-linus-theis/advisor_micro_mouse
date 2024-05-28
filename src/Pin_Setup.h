#include "Arduino.h"
#include "wiring.h"

//Pin renaming
//System
#define POWER_ENABLE    PC10  //Main Power Latch - enable directly after startup to keep Robot ON
#define V_BAT           PC0   //Battery Voltage divided by 1,47
#define SWD_IO          PA13  //Serial Wire Debug Pins
#define SWD_CLK         PA14

//Drive Motors & Encoders
#define MOTOR_nENABLE          PC11   //HIGH = Motor Driver disabled,   LOW = Motor Driver enabled !!!
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

void Pin_Init(void);


void Pin_Setup(void) {
//Pinmode Declaration
//Motors
  pinMode(MOTOR_nENABLE, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM_1, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM_2, OUTPUT);
  pinMode(MOTOR_LEFT_PWM_1, OUTPUT);
  pinMode(MOTOR_LEFT_PWM_2, OUTPUT);
  pinMode(MOTOR_ENCODER_RIGHT_A, INPUT);
  pinMode(MOTOR_ENCODER_RIGHT_B, INPUT);
  pinMode(MOTOR_ENCODER_LEFT_A, INPUT);
  pinMode(MOTOR_ENCODER_LEFT_B, INPUT);
  pinMode(MOTOR_nFAULT, INPUT);

//Servos
  pinMode(SERVO_ENABLE, OUTPUT);
  //pinMode(SERVO_PWM_1, OUTPUT);   //leave unconfigured as floating Input, until testet in Hardware!
  //pinMode(SERVO_PWM_2, OUTPUT);


//Infrared Emmitter
  pinMode(IR_EMITTER_RF, OUTPUT);
  pinMode(IR_EMITTER_LF, OUTPUT);
  pinMode(IR_EMITTER_RD, OUTPUT);
  pinMode(IR_EMITTER_LD, OUTPUT);
  pinMode(IR_EMITTER_RS, OUTPUT);
  pinMode(IR_EMITTER_LS, OUTPUT);
  pinMode(IR_EMITTER_MID, OUTPUT);

//Infrared Sensors
  pinMode(IR_SENSOR_RF, INPUT_ANALOG);
  pinMode(IR_SENSOR_LF, INPUT_ANALOG);
  pinMode(IR_SENSOR_RD, INPUT_ANALOG);
  pinMode(IR_SENSOR_LD, INPUT_ANALOG);
  pinMode(IR_SENSOR_RS, INPUT_ANALOG);
  pinMode(IR_SENSOR_LS, INPUT_ANALOG);
  pinMode(IR_SENSOR_MID, INPUT_ANALOG);


//IMU
  //->SPI #1
  //TO-DO !!!!! --------------------------------

  
//OLED Display
  pinMode(OLED_SCL, OUTPUT);
  pinMode(OLED_SCL, OUTPUT);
  
  
//Bluetooth Module
  pinMode(BLUETOOTH_RX, INPUT);
  pinMode(BLUETOOTH_TX, OUTPUT);
  

//User-Interface-Encoder
  pinMode(UI_BUTTON, INPUT_PULLUP);   //Pullup important (no Hardware Pullup upsi)
  pinMode(UI_ENCODER_A, INPUT);
  pinMode(UI_ENCODER_B, INPUT);
  

//Buzzer
  pinMode(BUZZER, OUTPUT);  
  
  
//Debug RGB LED
  pinMode(LED_RED, OUTPUT_OPEN_DRAIN);
  pinMode(LED_GREEN, OUTPUT_OPEN_DRAIN);
  pinMode(LED_BLUE, OUTPUT_OPEN_DRAIN);
  


  Pin_Init();
}


void Pin_Init() {
//Turn ON Power Latch 
  digitalWrite(POWER_ENABLE, HIGH);

//Turn OFF Motor Driver
  digitalWrite(MOTOR_nENABLE, HIGH);

//Turn OFF Debug LED
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}