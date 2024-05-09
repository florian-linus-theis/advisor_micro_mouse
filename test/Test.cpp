#include <iostream>
#include <Arduino.h>
#include "Pin_Init.h"

class Test{
    public:

    void Motoren_Test(void){
        digitalWrite(MOTOR_ENABLE, HIGH); // Motoren Enablen

        analogWrite(MOTOR_RECHTS_PWM_1, 150);
        analogWrite(MOTOR_RECHTS_PWM_2, 150);


        analogWrite(MOTOR_LINKS_PWM_1, 150);
        analogWrite(MOTOR_LINKS_PWM_2, 150);


    };

    void Sensoren_Test(void){
        Messung_Blind();
        Messung_Hell();
        
    }


    void UI_Test(){
        


    }

    void 





};


void setup(){


}

void loop(){


}