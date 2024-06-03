#pragma once
#include "Setup\Setup.h"
#include <cmath>

double last_time=0;
double dt, kp_default=2, ki_default=0, kd_default=0.001;
double proportional, integral, differential;
signed int previous=0;
int base_speed = 8 * 10;
int default_max_correction_speed = 30, max_correction_speed;

enum{
    ROT_ERROR = 0,
    X_ERROR = 1,
    Y_ERROR = 2,
    CURVE_LEFT_ERROR = 3,
    CURVE_RIGHT_ERROR = 4,
    X_ERROR_LEFT_WALL_ONLY = 5,
    X_ERROR_RIGHT_WALL_ONLY = 6
};

std::vector<int> NeutralSensorValues {150,78,330,230,60,144};
std::vector<signed int> ErrorSensorsVec {0,0,0,0,0,0};

 void debug_print(){
    // ble->print(": ");
    // ble->println(output_G);
    // ble->println("---");
    // ble->print("Danach: ");
    // ble->println(output_G);

 }
        

double calc_dt(){
    double now = millis();
    dt = now-last_time;
    last_time = now;
    return dt;
}

int applyPID(signed int error, double kp = kp_default, double ki = ki_default, double kd = kd_default){
    proportional = error;
    integral += error * dt;
    differential = (error-previous)/dt;
    previous = error;
    double output = kp * proportional + ki*integral + kd*differential;
    output = output/500;
    int output_G_int = static_cast<int>(round(output));
    return output_G_int;
}

int calcError(int PID_case){
    int error, desired_distance_L, desired_distance_R;
    switch (PID_case)
    {
    case ROT_ERROR:
        error = 0;
        break;

    case X_ERROR:
        error = (ErrorSensorsVec[0]+ErrorSensorsVec[1])-(ErrorSensorsVec[4]+ErrorSensorsVec[5]);
        break;

    case Y_ERROR:
        error = 0;
        break;

    case CURVE_LEFT_ERROR:
        desired_distance_L = (TICKS_INNER_WHEEL/TICKS_OUTER_WHEEL)*distance_traveled_R;
        error = distance_traveled_L - desired_distance_L;
        break;  

    case CURVE_RIGHT_ERROR:
        desired_distance_R = (TICKS_INNER_WHEEL/TICKS_OUTER_WHEEL)*distance_traveled_L;
        error = distance_traveled_R - desired_distance_R;
        break;

    case X_ERROR_LEFT_WALL_ONLY:
        error = (ErrorSensorsVec[0]+ErrorSensorsVec[1]);
        break;

    case X_ERROR_RIGHT_WALL_ONLY:
        error = -(ErrorSensorsVec[4]+ErrorSensorsVec[5]);
        break;

    default:
        error = 0;
        break;
    }

    return error;
}

int cap_output(int output_G, int max_correction_speed = default_max_correction_speed){
    if(output_G > max_correction_speed){
        output_G = max_correction_speed;
    }
    else if(output_G < -max_correction_speed){
        output_G = -max_correction_speed;
    }
    return output_G;
}

std::vector<int> calc_correction(int PID_case){
    signed int correction_left, correction_right;
    std::vector<signed int> correction_output;
    signed int output_G;

    switch (PID_case)
    {
    case ROT_ERROR:
        break;

    case X_ERROR:
        max_correction_speed = default_max_correction_speed; 
        output_G = applyPID(calcError(X_ERROR)); //<-- Case Specific kp,ki,kd-values can be defined here
        output_G = cap_output(output_G,max_correction_speed);
        correction_left = output_G;
        correction_right = -output_G;
        break;

    case Y_ERROR:
        break;

    case CURVE_LEFT_ERROR:
        max_correction_speed = default_max_correction_speed; 
        output_G = applyPID(calcError(CURVE_LEFT_ERROR));
        output_G = cap_output(output_G,max_correction_speed);
        correction_left = output_G;
        correction_right = 0;
        break;

    case CURVE_RIGHT_ERROR:
        max_correction_speed = default_max_correction_speed;
        output_G = applyPID(calcError(CURVE_RIGHT_ERROR));
        output_G = cap_output(output_G,max_correction_speed);
        correction_left = 0;
        correction_right = output_G;
        break;

    case X_ERROR_LEFT_WALL_ONLY:
        max_correction_speed = default_max_correction_speed;
        output_G = applyPID(calcError(X_ERROR_LEFT_WALL_ONLY));
        output_G = cap_output(output_G,max_correction_speed);
        correction_left = output_G;
        correction_right = -output_G;
        break;

    case X_ERROR_RIGHT_WALL_ONLY:
        max_correction_speed = default_max_correction_speed;
        output_G = applyPID(calcError(X_ERROR_RIGHT_WALL_ONLY));
        output_G = cap_output(output_G,max_correction_speed);
        correction_left = output_G;
        correction_right = -output_G;
        break;

    default:
        correction_left = 0;
        correction_right = 0;
        break;
    }

    correction_output = {correction_left,correction_right};
    return correction_output;
}


void calc_Sensor_Errors(){
    for(int i=0;i<6;i++){
        ErrorSensorsVec[i] = Distance_Sensor[i]-NeutralSensorValues[i];
        ble->print(i);
        ble->print(": ");
        ble->println(ErrorSensorsVec[i]);
    }
}

void pid_move_function(std::vector<int> correction){
    ForwardLeft(base_speed + correction[0]);
    ForwardRight(base_speed + correction[1]);
}



void PID_Test(){
    while(1){

        for(int i=0;i<6;i++){
            ble->print("Sensor ");
            ble->print(i);
            ble->print(": ");
            ble->println(Distance_Sensor[i]);
        }
        ble->println("");

        delay(1000);
        

        // //Enable Motors
        // digitalWrite(MOTOR_ENABLE, LOW);

        // //Calculate dt for Integral Component of PID
        // dt = calc_dt();

        // //Calculate deviation from NeutralSensorValues (Error = 0 --> Mous is perfectly centered and oriented)
        // calc_Sensor_Errors();


        // //Calculate corrections and set motors accordingly
        // pid_move_function(calc_correction(X_ERROR_LEFT_WALL_ONLY));



        if(encoderTurned){
            encoderTurned=false;
            digitalWrite(MOTOR_ENABLE, HIGH);
            break;
        }
    }


}