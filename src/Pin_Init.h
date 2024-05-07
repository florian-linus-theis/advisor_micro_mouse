#include <Arduino.h>
#include <wiring.h>

//Portbelegung und Umbenennung
//Antriebs Motoren
#define MOTOR_ENABLE            PC11
#define MOTOR_RECHTS_PWM_1      PB7     //Belegung nicht final, da Drehrichtung noch unbekannt
#define MOTOR_RECHTS_PWM_2      PB6     //*
#define MOTOR_LINKS_PWM_1       PB4     //*
#define MOTOR_LINKS_PWM_2       PB5     //*
#define MOTOR_ENCODER_RECHTS_A  PA0     //Belegung nicht final, da Drehrichtung noch unbekannt
#define MOTOR_ENCODER_RECHTS_B  PA1     //*
#define MOTOR_ENCODER_LINKS_A   PA15    //*
#define MOTOR_ENCODER_LINKS_B   PB3     //*
#define MOTOR_nFAULT            PD2

//Servos
#define SERVO_ENABLE    PH1
#define SERVO_PWM_1     PB8
#define SERVO_PWM_2     PB9

//Infrarot Emmitter
#define IR_EMITTER_RF   PC8
#define IR_EMITTER_LF   PC9
#define IR_EMITTER_RD   PC6
#define IR_EMITTER_LD   PC7
#define IR_EMITTER_RS   PB14
#define IR_EMITTER_LS   PB15
#define IR_EMITTER_MID  PB13

//Infrarot Sensoren
#define IR_SENSOR_RF    PC2
#define IR_SENSOR_LF    PB1
#define IR_SENSOR_RD    PA2
#define IR_SENSOR_LD    PB0
#define IR_SENSOR_RS    PA3
#define IR_SENSOR_LS    PC5
#define IR_SENSOR_MID   PC4

//IMU
#define IMU_SCL         PA5
#define IMU_MOSI        PA7
#define IMU_MISO        PA6
#define IMU_nCS         PA4
#define IMU_INT         PA8
#define IMU_FSYNC       PB12

//OLED Display
#define OLED_SCL        PB10
#define OLED_SDA        PB11

//Bluetooth Modul
#define BLUETOOTH_TX    PA9
#define BLUETOOTH_RX    PA10

//User-Interface-Encoder
#define UI_ENCODER_A    PH0
#define UI_ENCODER_B    PC15
#define UI_BUTTON       PC12

//Buzzer
#define BUZZER          PA11

//Debug RGB LED
#define LED_ROT         PC1
#define LED_GRÜN        PA12
#define LED_BLAU        PC3

//Sonstige Belegungen
#define POWER_ENABLE    PC10
#define V_BAT           PC0
#define SWD_IO          PA13
#define SWD_CLK         PA14



//Deklaration der Pinmodi
void Pin_Setup(void){
//Motoren

//Servos

//Infrarot Emmitter
  pinMode(IR_EMITTER_RF, OUTPUT);
  pinMode(IR_EMITTER_LF, OUTPUT);
  pinMode(IR_EMITTER_RD, OUTPUT);
  pinMode(IR_EMITTER_LD, OUTPUT);
  pinMode(IR_EMITTER_RS, OUTPUT);
  pinMode(IR_EMITTER_LS, OUTPUT);
  pinMode(IR_EMITTER_MID, OUTPUT);

//Infrarot Sensoren
  pinMode(IR_SENSOR_RF, INPUT_ANALOG);
  pinMode(IR_SENSOR_LF, INPUT_ANALOG);
  pinMode(IR_SENSOR_RD, INPUT_ANALOG);
  pinMode(IR_SENSOR_LD, INPUT_ANALOG);
  pinMode(IR_SENSOR_RS, INPUT_ANALOG);
  pinMode(IR_SENSOR_LS, INPUT_ANALOG);
  pinMode(IR_SENSOR_MID, INPUT_ANALOG);

//IMU
  //->SPI #1
  
  
//OLED Display
  //->I2C #2
  
  
//Bluetooth Modul
  //->USART #1
  

//User-Interface-Encoder
  //->read over Interrupt
  

//Buzzer
  
  
//Debug RGB LED
  pinMode(LED_ROT, OUTPUT_OPEN_DRAIN); //RGB LED Pinmode muss OUTPUT_OPEN_DRAIN sein !!!!!!!!!!!
  pinMode(LED_GRÜN, OUTPUT_OPEN_DRAIN);
  pinMode(LED_BLAU, OUTPUT_OPEN_DRAIN);


//Sonstige Belegungen
  
    
}
