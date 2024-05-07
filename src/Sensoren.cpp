#include <iostream>
#include <vector>
#include "Arduino.h"
#include "wiring.h"
#include "Pin_Init.h"


int Channel[] = {IR_EMMITER_LS, IR_EMMITER_LD, IR_EMMITER_LF, IR_EMMITER_RF, IR_EMMITER_RD, IR_EMMITER_RS};
vec<int> Messung_Blind;
vec<int> MEssung_Hell;
int* Messung = &Channel[0];

class Sensoren{
    void Sensor_Messung_Umgebung_Blind(void){
        for(int i = 0; i< 6; i++){
            Messung_Blind.push_back(analogRead(Channel[i]));
        }
    }

};
