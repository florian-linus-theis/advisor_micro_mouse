#include <iostream>
#include <vector>
#include "Arduino.h"
#include "wiring.h"
#include "Pin_Init.h"


class Sensoren{
  public:
      int interrupt_counter = 0;
      bool Sensor_Error;
        //Messdaten werden gespeichert, Array dient als Hilfe zur Abfrage der Sensoren und zur Definition der Kanäle
      int Channel_Emitter[] = {IR_EMITTER_LS, IR_EMITTER_LD, IR_EMITTER_LF, IR_EMITTER_RF, IR_EMITTER_RD, IR_EMITTER_RS};
      int Channel_Sensoren[] = {IR_SENSOR_LS, IR_SENSOR_LD, IR_SENSOR_LF, IR_SENSOR_RF, IR_SENSOR_RD, IR_SENSOR_RS};
      vec<int> Messung_Blind;
      vec<int> Messung_Hell;
      vec<int> Distanz_Sensoren_MM;


    void Distanz_Messung_Blind(void){
        for(int i = 0; i< 6; i++){
            Messung_Blind.push_back(analogRead(Channel_Emitter[6 - i]));
        }
    }

    void Distanz_Messung_Hell(void){
      Sensor_Error = false;        //Error-Flag
      interrupt_counter = 0;
      digitalWrite(IR_SENSOR_RS,HIGH);
      while(interrupt_counter !=7){
      timer4.resume();                // Start
      }
      if(interrupt_counter == 7){
        Sensor_Feedback = false;
        timer4.pause();
      }
      else{
        //Fehler ist aufgetreten
        Sensor_Error = true;
      }
    }
};

