#include "Setup.h"

HardwareSerial *ble = nullptr;
//HardwareSerial *ble;

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
  //HardwareSerial *ble = &Serial1;
  //ble = &Serial1;  // Beispiel: Verwenden von Serial1
  HardwareSerial *ble = new HardwareSerial(BLUETOOTH_RX, BLUETOOTH_TX);
  ble->begin(115200);
  

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
//Sontiges
  pinMode(POWER_ENABLE, OUTPUT);
  


  Set_Output();
}



void Set_Output(void) {
//Turn ON Power Latch 
  digitalWrite(POWER_ENABLE, HIGH);

//Turn OFF Motor Driver
  digitalWrite(MOTOR_nENABLE, LOW);

//Turn OFF Debug LED
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}