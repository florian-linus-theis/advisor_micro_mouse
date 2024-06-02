#pragma once
#include "Setup\Setup.h"
#include <cmath>

double last_time=0;
double dt, kp=2, ki=0, kd=0.001;
double proportional, integral, differential;
signed int previous=0;
int max_speed = 3 * 10;
int base_speed = 8 * 10;

std::vector<int> NeutralSensorValues {150,78,330,230,60,144};
std::vector<signed int> ErrorSensorsVec {0,0,0,0,0,0};

double applyPID(signed int error){
    proportional = error;
    integral += error * dt;
    differential = (error-previous)/dt;
    previous = error;
    double output = kp * proportional + ki*integral + kd*differential;
    output = output/500;
    return output;
}


void PID_Test(){
    double output_G = 0;
    ble->println("PID_TEST started");


    while(1){
        digitalWrite(MOTOR_ENABLE, LOW);

        double now = millis();
        dt = now-last_time;
        last_time = now;
        for(int i=0;i<6;i++){
            ErrorSensorsVec[i] = Distance_Sensor[i]-NeutralSensorValues[i];
            ble->print(i);
            ble->print(": ");
            ble->println(ErrorSensorsVec[i]);
        }

        //signed int rot_error = -ErrorSensorsVec[2]+ErrorSensorsVec[3];
        signed int x_error = (ErrorSensorsVec[0]+ErrorSensorsVec[1])-(ErrorSensorsVec[4]+ErrorSensorsVec[5]);
        output_G = applyPID(x_error) * 10;
        ble->print("Output_G: ");
        ble->println(output_G);
        ble->println("---");
        if(output_G > max_speed){
            output_G = max_speed;
        }
        else if(output_G < -max_speed){
            output_G = -max_speed;
        }
        else{
            output_G = output_G;
        }
        ble->print("Danach: ");
        ble->println(output_G);
        
        // ble->print("Rotation: ");
        // ble->println(rot_error);
        // ble->print("Sensor2: ");
        // ble->println(Distance_Sensor[2]);
        // ble->print("Sensor3: ");
        // ble->println(Distance_Sensor[3]);0
        

        delay(100);

        signed int output_G_int = static_cast<int>(round(output_G));
        // if(abs(x_error) < 1){
        //     continue;
        // }

        // if(output_G == -max_speed){
        //     ble->println("Rotate right fast");
        //     ForwardLeft(base_speed-max_speed);
        //     ForwardRight(base_speed+max_speed);
        //     // BackwardLeft(max_speed);
        //     // ForwardRight(max_speed);           
        // }
        // else if(output_G == max_speed){
        //     ble->println("Rotate left fast");
        //     ForwardLeft(base_speed+max_speed);
        //     ForwardRight(base_speed-max_speed);
        //     // ForwardLeft(max_speed);
        //     // BackwardRight(max_speed);
        // }
        if(0 < output_G){
            ble->println("Rotate left normal");
            ForwardLeft(base_speed+output_G_int);
            ForwardRight(base_speed-output_G_int);
            // ForwardLeft(4*output_G_int);
            // BackwardRight(4*output_G_int);
        }
        else if(output_G < 0){
            ble->println("Rotate right normal");
            ForwardLeft(base_speed+output_G_int);
            ForwardRight(base_speed-output_G_int);
            // BackwardLeft(-output_G_int*4);
            // ForwardRight(-output_G_int*4);
        }
        else{
            ble->println("Move straight ");
            ForwardLeft(base_speed);
            ForwardRight(base_speed);
            // BackwardLeft(-output_G_int*4);
            // ForwardRight(-output_G_int*4);
        }

        if(encoderTurned){
            encoderTurned=false;
            digitalWrite(MOTOR_ENABLE, HIGH);
            break;
        }
    }


}