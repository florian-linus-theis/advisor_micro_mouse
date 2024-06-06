#pragma once 
#include <setup\setup.h>

int adc_value; 
float battery_volts;  
int battery_percentage; 
const float v_min = 2.0;
const float v_max = 2.7;          

//Spannung an Batterie in Volt
float voltage() {
    adc_value = analogRead(V_BAT); 
    battery_volts = 3.3/4096.0/1.47;
    return battery_volts;
}
//Batterie-Status in % 
int battery_status() {
    battery_percentage = (battery_volts - v_min)/(v_max - v_min) * 100;
    return battery_percentage;
    }