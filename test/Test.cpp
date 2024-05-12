#include <iostream>
#include <Arduino.h>
#include "Pin_Init.h"
#include "Sensoren.cpp"

class Test{
    public:

    void Motoren_Test(void){
        digitalWrite(MOTOR_ENABLE, HIGH); // Motoren Enablen

        analogWrite(MOTOR_RECHTS_PWM_1, 150);
        analogWrite(MOTOR_RECHTS_PWM_2, 150);


        analogWrite(MOTOR_LINKS_PWM_1, 150);
        analogWrite(MOTOR_LINKS_PWM_2, 150);


    };

    Sensoren sensoren;

    void Sensoren_Test(void){
        sensoren.Distanz_Messung_Blind(Messung_Blind);
        sensoren.Distanz_Messung_Hell(Messung_Hell);
        
    }


    void UI_Test(){
        


    }
};


void setup(){


}

void loop(){


}