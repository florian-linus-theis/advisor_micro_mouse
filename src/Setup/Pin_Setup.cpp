#include "Setup.h"


SPIClass *imu = nullptr;
HardwareSerial *ble = nullptr;


void Pin_Setup(void) {  //Pinmode Declaration and Communication Port Initialization
//System
  pinMode(POWER_ENABLE, OUTPUT);
  pinMode(V_BAT, INPUT_ANALOG);


//Motors
  pinMode(MOTOR_ENABLE, OUTPUT);
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
  pinMode(SERVO_PWM_1, OUTPUT);   
  //pinMode(SERVO_PWM_2, OUTPUT); //leave unconfigured as floating Input


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



//IMU -> SPI #1
  pinMode(IMU_SCL, OUTPUT);
  pinMode(IMU_MOSI, OUTPUT);
  pinMode(IMU_MISO, INPUT);
  pinMode(IMU_nCS, OUTPUT);
  pinMode(IMU_INT, INPUT);
  pinMode(IMU_FSYNC, OUTPUT);
/*
  digitalWrite(IMU_nCS, HIGH);
  imu = new SPIClass(IMU_MOSI, IMU_MISO, IMU_SCL, IMU_nCS);
  //imu->begin(); //neccesary or not ... only GOD knows
  imu->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

  delay(1);   //wait 11mS (max 100mS) for IMU Bootup
  //set I2C_IF_DIS Bit (#4) in USER_CTRL Register (#106) to disable I2C Interface
  u_int8_t buf[] = {128+106, 0b00010000};
  digitalWrite(IMU_nCS, LOW);
  imu->transfer(buf, sizeof(buf), true);
  digitalWrite(IMU_nCS, HIGH);
  delay(1);

  //set DEVICE_RESET Bit (#7) in PWR_MGMT_1 Register (#107) to reset all Registers
  buf[0] = 128+107; buf[1] = 0b10000000;
  digitalWrite(IMU_nCS, LOW);
  imu->transfer(buf, sizeof(buf), true);
  digitalWrite(IMU_nCS, HIGH);
  delay(100); //wait for reset

  //set I2C_IF_DIS Bit (#4) and SIG_COND_RST Bit (#0) in USER_CTRL Register (#106) to disable I2C Interface again and Reset all Digital Signal Paths
  buf[0] = 128+106; buf[1] = 0b00010001;
  digitalWrite(IMU_nCS, LOW);
  imu->transfer(buf, sizeof(buf), true);
  digitalWrite(IMU_nCS, HIGH);
  //delay(100);
*/  


//OLED Display
  pinMode(OLED_SCL, OUTPUT);
  pinMode(OLED_SCL, OUTPUT);
  
  

//Bluetooth Module
  pinMode(BLUETOOTH_RX, INPUT);
  pinMode(BLUETOOTH_TX, OUTPUT);
  //HardwareSerial *ble = &Serial1;
  //ble = &Serial1;  // Beispiel: Verwenden von Serial1
  ble = new HardwareSerial(USART1);
  //HardwareSerial *ble = new HardwareSerial(BLUETOOTH_RX, BLUETOOTH_TX);
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

  
//Set normally HIGH Outputs
  Set_Output();
}



void Set_Output(void) {
//Turn ON Power Latch 
  digitalWrite(POWER_ENABLE, HIGH);

//Turn OFF Motor Driver
  digitalWrite(MOTOR_ENABLE, HIGH);

//Turn OFF Debug LED
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  // Servo
  digitalWrite(SERVO_ENABLE, HIGH);
}