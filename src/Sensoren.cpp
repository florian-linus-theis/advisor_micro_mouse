#include <iostream>
#include <vector>
#include "Arduino.h"
#include "wiring.h"
#include "Pin_Init.h"
using std::vector;

volatile int interrupt_counter = 0;
bool Sensor_Feedback;
//Messdaten werden gespeichert, Array dient als Hilfe zur Abfrage der Sensoren und zur Definition der Kanäle
int Channel_Emitter[6] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS};
int Channel_Sensoren[6] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS};
vector<int> Messung_Blind;
vector<int> Messung_Hell;
vector<int> Distanz_Sensoren_MM;


class Sensoren{
  public:
      
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
};

