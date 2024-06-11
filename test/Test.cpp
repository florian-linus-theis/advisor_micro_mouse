#include <Arduino.h>
#include <Setup/Setup.h>
using std::vector;


//Variablen und Container
HardwareTimer timer4(TIM4);

volatile int interrupt_counter;
volatile int Drehzahl_Links;
volatile int Drehzahl_Rechts;

bool Sensor_Feedback;
int Channel_Emitter[6] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS};
int Channel_Sensoren[6] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS};
vector<int> Messung_Blind;
vector<int> Messung_Hell;
vector<int> Distanz_Sensoren_MM;

// Funktionennamen
void Timer4_Setup(void);
void Sensor_Interrupt(void);
void Distanz_Messung_Blind(void);
void Distanz_Messung_Hell(void);
void Drehzahl_Interrupt_LinksA(void);
void Drehzal_Interrupt_RechtsA(void);
void Motor_Fehler(void);

//Software setup
void setup() {
  Timer4_Setup();
  Pin_Setup();
}


//Hauptschleife
void loop() {
  digitalWrite(POWER_ENABLE, HIGH);
  digitalWrite(MOTOR_ENABLE, HIGH);
  Distanz_Messung_Blind(Messung_Blind);
  Distanz_Messung_Hell(Messung_Hell);
  for(int i = 0; i < 6; i++){
    if(Messung_Blind[i] == Messung_Hell[i]){
      Sensor_Feedback = true;
    }
    else{
      Sensor_Feedback = false;
    }
    //Messwerte ausgeben
    Serial.begin(9600);
    Serial.print(Channel_Sensoren[i]);
    Serial.print(Messung_Hell[i]-Messung_Blind[i]);
    Serial.print(": Messergebnis");
  }
  if(Sensor_Feedback = true){
    Serial.print("Messung Sensoren nicht erfolgreich");   //Default
  }
  else{
    Serial.print("Messung Sensoren erfolgreich!");
  }

  delay(500);

  attachInterrupt(MOTOR_ENCODER_LEFT_A, Drehzahl_Interrupt_LinksA, RISING);
  attachInterrupt(MOTOR_ENCODER_RIGHT_A, Drehzahl_Interrupt_RechtsA, RISING);
  attachInterrupt(MOTOR_nFAULT, Motor_Fehler, FALLING);

  analogWrite(MOTOR_LEFT_PWM_1, 150);
  analogWrite(MOTOR_RIGHT_PWM_1, 150);

  delay(1000);
  analogWrite(MOTOR_LEFT_PWM_1, 0);
  analogWrite(MOTOR_RIGHT_PWM_1, 0);
  analogWrite(MOTOR_LEFT_PWM_2, 150);
  analogWrite(MOTOR_RIGHT_PWM_2, 150);

  delay(1000);

  analogWrite(MOTOR_LEFT_PWM_2, 0);
  analogWrite(MOTOR_RIGHT_PWM_2, 0);

  if((Drehzahl_Links) != 0 && (Drehzahl_Rechts != 0)){
    Serial.print("Motoren werden erfolgreich angesteuert und ausgelesen");
  }
  else{
    Serial.print("Mororen können nicht erfolreich angesteuert oder ausgelesen werden");
    Sensor_Feedback = true;
  }

  if(Sensor_Feedback == false){
    Serial.print("Prüfroutine erfolgreich abgeschlossen");
  }
  else{
    Serial.print("Es ist ein Fehler in der Prüfroutine aufgetreten");
  }

  Serial.print("Prüfroutine Ende, Motor-Fehler kann bei erfolgreicher Prüfung ignoriert werden");
  Motor_Fehler();
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
} //Ende der Main-Schleife


//Sensorenansteuerung
void Timer4_Setup(void){
// Configure timer
  timer4.setPrescaleFactor(50); // Set prescaler to 50
  timer4.setOverflow(16);      // Set overflow to 16 = 50us Intervalle
  timer4.attachInterrupt(Sensor_Interrupt);
  timer4.refresh();
  timer4.resume();
  NVIC_EnableIRQ(TIM4_IRQn);
}

void Sensor_Interrupt() {
  Messung_Hell.push_back(analogRead(Channel_Sensoren[interrupt_counter])); //Array wird von hinten nach vorne durchlaufen,sodass beim pushback nicht die Reihenfolge verfälscht wird
  digitalWrite(Channel_Emitter[interrupt_counter], LOW);
  interrupt_counter++;
  if(interrupt_counter == 7){
    digitalWrite(Channel_Sensoren[interrupt_counter], HIGH);
  }
}

void Distanz_Messung_Blind(vector<int>&Messung_Blind){
  Messung_Blind.clear();
    for(int i = 0; i< 6; i++){
        Messung_Blind.push_back(analogRead(Channel_Emitter[i]));
    }
}

void Distanz_Messung_Hell(vector<int>&Messung_Hell){
  Messung_Hell.clear();
  Sensor_Feedback = false;        //Error-Flag
  interrupt_counter = 0;


  digitalWrite(IR_SENSOR_RS,HIGH);        //Messbeginn: LED ON - 50us - Sensor auslesen
  while(interrupt_counter !=7){
  NVIC_EnableIRQ(TIM4_IRQn);              
  }


  if(interrupt_counter == 7){
    Sensor_Feedback = false;
    NVIC_DisableIRQ(TIM4_IRQn);
  }
  else{
    //Fehler ist aufgetreten
    Sensor_Feedback = true;
  }
}

void Drehzahl_Interrupt_LinksA(){
  Drehzahl_Links++;
}
void Drehzahl_Interrupt_RechtsA(){
  Drehzahl_Rechts++;
}


void Motor_Fehler(){
  digitalWrite(MOTOR_LEFT_PWM_1, LOW);
  digitalWrite(MOTOR_LEFT_PWM_2, LOW);
  digitalWrite(MOTOR_RIGHT_PWM_1, LOW);
  digitalWrite(MOTOR_RIGHT_PWM_2, LOW);
  Serial.print("Motor-Fehler aufgetreten, Maus abgeschalten");
  digitalWrite(MOTOR_ENABLE, LOW);
  digitalWrite(POWER_ENABLE, LOW);
  digitalWrite(LED_RED, LOW);           // LOW == Einschalten
}