#pragma once
#include "Setup\Setup.h"

double last_time=0;
double dt, kp=3, ki=0.01, kd=0.001;
double proportional, integral, differential;
signed int previous=0;
int max_speed = 5;

std::vector<int> NeutralSensorValues {100,100,330,230,100,100};

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
        signed int ErrorSensor2 = Distance_Sensor[2]-NeutralSensorValues[2];
        signed int ErrorSensor3 = Distance_Sensor[3]-NeutralSensorValues[3];


        signed int rot_error = -ErrorSensor2+ErrorSensor3;

        output_G = applyPID(rot_error);
        // ble->print("Rotation: ");
        // ble->println(rot_error);
        ble->print("Sensor2: ");
        ble->println(Distance_Sensor[2]);
        ble->print("Sensor3: ");
        ble->println(Distance_Sensor[3]);
        ble->print("Output_G: ");
        ble->println(output_G);
        ble->println("---");

        delay(100);

        int output_G_int = static_cast<int>(round(output_G));

        if(output_G < -max_speed){
            ble->println("Rotate right fast");
            BackwardLeft(max_speed);
            ForwardRight(max_speed);           
        }
        else if(output_G > max_speed){
            ble->println("Rotate left fast");
            ForwardLeft(max_speed);
            BackwardRight(7);
        }
        else if(0 < output_G && output_G < max_speed){
            ble->println("Rotate left normal");
            ForwardLeft(2*output_G_int);
            BackwardRight(2*output_G_int);
        }
        else if(output_G < 0 && output_G > -max_speed){
            ble->println("Rotate right normal");
            BackwardLeft(-output_G_int*2);
            ForwardRight(-output_G_int*2);
        }




        if(encoderTurned){
            encoderTurned=false;
            digitalWrite(MOTOR_ENABLE, HIGH);
            break;
        }
    }


}