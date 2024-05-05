#include <Arduino.h>
#include <wiring.h>

//Definitionen der Bauteile und Ports

//IR Sensoren und Emmiter
#define IR_SENSOR_RF    PC8
#define IR_SENSOR_LF    PC9
#define IR_SENSOR_RD    PC6
#define IR_SENSOR_LD    PC7
#define IR_SENSOR_RS    PB14
#define IR_SENSOR_LS    PB15
#define IR_SENSOR_MID   PB13
#define IR_EMMITER_RF   PC2
#define IR_EMMITER_LF   PB1
#define IR_EMMITER_RD   PA2
#define IR_EMMITER_LD   PB0
#define IR_EMMITER_RS   PA3
#define IR_EMMITER_LS   PC5
#define IR_EMMITER_MID  PC4

//User Interface
#define OLED_SDA            PB11
#define OLED_SCL            PB10
#define POTENTIOMETER_UI_A  PH0
#define POTENTIOMETER_UI_B  PH15
#define SELECT_BUTTON_UI    PC12
#define BUZZER              PA11
#define RGB_LED_GELB        PA12
#define RGB_LED_BLAU        PA1
#define RGB_LED_GRÜN        PA0

//Motor
#define ENCODE_MOTOR_RECHTS_A   PA0
#define ENCODE_MOTOR_RECHTS_B   PA1
#define ENCODE_MOTOR_LINKS_A    PA15
#define ENCODE_MOTOR_LINKS_B    PB3
#define MOTOR_RECHTS_PWM_1      PB7
#define MOTOR_RECHTS_PWM_2      PB6
#define MOTOR_LINKS_PWM_1       PB4
#define MOTOR_LINKS_PWM_2       PB5
#define MOTOR_ENABLE            PC11
#define MOTOR_FAULT             PD2
#define SERVO1                  PB8
#define SERVO2                  PB9

//Sonstige Belegungen
#define BATTERY_VOLT            PC0
#define BLUETOOTH_MODUL_TX      PA9
#define BLUETOOTH_Module_RX     PA10
#define SERIAL_DEBUG_IO         PA13
#define SERIAL_DEBUG_CLK        PA14
#define IMU_FSYNC               PB12
#define IMU_MOSI                PA7
#define IMU_MISO                PA6
#define IMU_SCL                 PA5
#define IMU_CS                  PA4
#define IMU_INT                 PA8
#define IMU_FSYNC               PB12
#define POWER_ENABLE            PC10

//Deklaration der Pinfunktionen
void Pin_Setup(void){
//IR Sensoren und Emmiter
pinMode(IR_SENSOR_RF, OUTPUT);
pinMode(IR_EMMITER_RF, INPUT_ANALOG);

//User Interface

//Motor

//Sonstige Belegugungen

}


