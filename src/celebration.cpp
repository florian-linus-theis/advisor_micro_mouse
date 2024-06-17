#include "Setup/Setup.h"

void celebrate() {
    while(encoderTurned == false){
        digitalWrite(MOTOR_ENABLE, LOW);
        
        for(int i = 0; i < 3; i++){
            if(i == 0){
                digitalWrite(LED_RED, LOW); // on
                delay(500);
                digitalWrite(LED_RED, HIGH); // off
                }
            else if(i == 1){
                digitalWrite(LED_BLUE, LOW);
                delay(500);
                digitalWrite(LED_BLUE, HIGH); 
                }
            else if(i == 2){
                digitalWrite(LED_GREEN, LOW);
                delay(500);
                digitalWrite(LED_GREEN, HIGH); 
            }
            
            timer10->resume();
            digitalWrite(SERVO_ENABLE, HIGH);
            move_ballgrabber_forward();
            delay(300);
            move_ballgrabber_backward();
            delay(400);
            timer10->pause();
            digitalWrite(SERVO_ENABLE, LOW);
            rotate_left();
            delay(400);
        }
        digitalWrite(MOTOR_ENABLE, HIGH);
        //Gas_song_short();
    }
}