//Include Librarys
#include "Arduino.h"
#include "wiring.h"
#include "iostream"
#include "vector"
#include "cmath"
#include "HardwareTimer.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


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


//Function and Variable Initialisation - - - - - - - - - - - - - - - - - - - - -
//Pin_Setup
extern void Pin_Setup(void);
extern void Set_Output(void);


//Clock_Setup
extern void Clock_Setup();


//Timer_Setup
extern HardwareTimer *timer14;
extern HardwareTimer *timer3;
extern HardwareTimer *timer4;
extern HardwareTimer *timer2;
extern HardwareTimer *timer5;
extern HardwareTimer *timer6;
extern HardwareTimer *timer7;
extern HardwareTimer *timer10;
extern HardwareTimer *timer1;

extern void Timer_Setup(void);


//Systick
extern void Systick_Interrupt(void);
extern void update(void);



//ADC_Setup
extern void ADC_Setup(void);


//User-Interface
//Bluetooth
extern HardwareSerial *ble;



//Move to new Header File - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Sensors
extern void Timer6_Interrupt(void);
extern void Timer7_Interrupt(void);

extern void Distanz_Messung_Sensoren(void);

extern void Distanz_Mid_Sensor(void);
extern void printDistanzSensoren(void);
extern int Distance_Sensor_Mid_MM;


//Motors
extern uint32_t encoder_value_left;
extern int encoder_value_right;
extern int read_encoder_left_ticks(void);
extern int read_encoder_right_ticks(void);

//Move
extern void Forward(int dutyCycle);