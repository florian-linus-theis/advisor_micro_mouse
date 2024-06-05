#pragma once
#include "Setup\Setup.h"
#include <cmath>

double last_time=0;
double dt, kp_default=2, ki_default=0, kd_default=0.001;
double proportional, integral, differential;
signed int previous=0;
int base_speed = 100;
int default_max_correction_speed = 200; 
int max_correction_speed = 200;
int current_state;
int toggle_drive;
int LAST_CASE_PID = -1;

enum{
    ROT_ERROR = 0,
    X_ERROR = 1,
    X_ERROR_LEFT_WALL_ONLY = 2,
    X_ERROR_RIGHT_WALL_ONLY = 3,
    Y_ERROR = 4,
    ROTATE_LEFT = 5,
    ROTATE_RIGHT = 6,
    CURVE_LEFT_ERROR = 7,
    CURVE_RIGHT_ERROR = 8,
    BLIND = 9
};

int CURRENT_CASE_PID = X_ERROR_LEFT_WALL_ONLY;

std::vector<int> NeutralSensorValues {532, 470, 803, 678, 672, 475, 1423};
std::vector<int> ErrorSensorsVec {0,0,0,0,0,0};

 void debug_print(){
    // ble->print(": ");
    // ble->println(output_G);
    // ble->println("---");
    // ble->print("Danach: ");
    // ble->println(output_G);

    for(int i=0;i<6;i++){
            ble->print(i);
            ble->print(": ");
            ble->println(Distance_Sensor[i]);
            if(i==1){
                i+=2;
            }
        }
    ble->println("");

    // ble->print("2: ");
    // ble->println(Distance_Sensor[0]);
    // ble->print("3: ");
    // ble->println(Distance_Sensor[1]);


 }     

double calc_dt(){
    double now = millis();
    dt = now-last_time;
    last_time = now;
    return dt;
}

//Calculate offset from neutral sensor values
void calc_Sensor_Errors(){
    for(int i=0;i<6;i++){
        ErrorSensorsVec[i] = Distance_Sensor[i]-NeutralSensorValues[i];
        // ble->print(i);
        // ble->print(": ");
        // ble->println(ErrorSensorsVec[i]);
    }
}

//adjust possible correction according to speed
double give_percent(double value) {
    return -100 + 200 * ((value +5000) / 10000);
}

//Calculate error for PID (dependent on current case)
int calcError(int PID_case){
    int error, desired_distance_L, desired_distance_R;
    switch (PID_case)
    {
    case ROT_ERROR:
        error = 0;
        break;

    case X_ERROR:
        error = -(ErrorSensorsVec[0]+ErrorSensorsVec[1])+(ErrorSensorsVec[4]+ErrorSensorsVec[5]);
        error = error/4;
        break;

    case X_ERROR_LEFT_WALL_ONLY:
        error = (ErrorSensorsVec[0]+ErrorSensorsVec[1]);
        error = error/2;
        break;

    case X_ERROR_RIGHT_WALL_ONLY:
        error = -(ErrorSensorsVec[4]+ErrorSensorsVec[5]);
        error = error/2;
        break;

    case Y_ERROR:
        error = (ErrorSensorsVec[2]+ErrorSensorsVec[3]);
        error = error/2;
        break;

    case ROTATE_LEFT:
        error = 0;
        break;

    case ROTATE_RIGHT:
        error = 0;
        break;

    case CURVE_LEFT_ERROR:
        desired_distance_L = 0.475 * distance_traveled_R;
        error = distance_traveled_L - desired_distance_L;
        break;  

    case CURVE_RIGHT_ERROR:
        desired_distance_R = (TICKS_INNER_WHEEL/TICKS_OUTER_WHEEL)*distance_traveled_L;
        error = distance_traveled_R - desired_distance_R;
        break;

    default:
        error = 0;
        break;
    }

    return error;
}

int applyPID(signed int error, double kp = 3, double ki = 0.0001, double kd = 0.002){
    if(CURRENT_CASE_PID != LAST_CASE_PID){
        kd = 0;    
    }
    LAST_CASE_PID = CURRENT_CASE_PID;
    error = give_percent(error);
    proportional = error;
    
    if(abs(integral) < 100000){
        integral += error * dt;
    }
    // if((abs(integral*ki) < 7 && ki > 0.001) || abs(error) < 5){
    //     output = 0;
    //     toggle_drive = 0;

    // }
    // else{
    differential = (error-previous)/dt;
    double output = kp * proportional + ki*integral + kd*differential;
    // }
    // toggle_drive = 1;
    //ble->println(output);
    previous = error;
    int output_G_int = static_cast<int>(round(output));
    return output_G_int;
}

//cap max correction speed
int cap_output(int output_G){
    if(output_G > max_correction_speed){
        output_G = max_correction_speed;
    }
    else if(output_G < -max_correction_speed){
        output_G = -max_correction_speed;
    }
    return output_G;
}

std::vector<int> calc_correction(int PID_case){
    dt = calc_dt();
    calc_Sensor_Errors();
    signed int correction_left, correction_right;
    std::vector<signed int> correction_output;
    signed int output_G;

    switch (PID_case)
    {
    case ROT_ERROR:
        break;
        
    case X_ERROR:
        max_correction_speed = default_max_correction_speed;
        output_G = applyPID(calcError(X_ERROR),2,0.0001,0.01); //<-- Case Specific kp,ki,kd-values can be defined here
        output_G = cap_output(output_G);
        correction_left = -output_G;
        correction_right = output_G;
        break;

    case Y_ERROR:
        max_correction_speed = default_max_correction_speed; 
        output_G = applyPID(calcError(Y_ERROR),2,0.0001,0.01);
        output_G = cap_output(output_G);
        correction_left = -output_G;
        correction_right = -output_G;
        break;

    case CURVE_LEFT_ERROR:
        max_correction_speed = default_max_correction_speed; 
        output_G = applyPID(calcError(CURVE_LEFT_ERROR));
        output_G = cap_output(output_G);
        correction_left = output_G;
        correction_right = 0;
        break;

    case CURVE_RIGHT_ERROR:
        max_correction_speed = default_max_correction_speed;
        output_G = applyPID(calcError(CURVE_RIGHT_ERROR));
        output_G = cap_output(output_G);
        correction_left = 0;
        correction_right = output_G;
        break;

    case X_ERROR_LEFT_WALL_ONLY:
        max_correction_speed = default_max_correction_speed;
        output_G = applyPID(calcError(X_ERROR_LEFT_WALL_ONLY),12,0.0001,0.01);
        output_G = cap_output(output_G);
        correction_left = output_G;
        correction_right = -output_G;
        break;

    case X_ERROR_RIGHT_WALL_ONLY:
        max_correction_speed = default_max_correction_speed;
        output_G = applyPID(calcError(X_ERROR_RIGHT_WALL_ONLY),12,0.0001,0.01);
        output_G = cap_output(output_G);
        correction_left = output_G;
        correction_right = -output_G;
        break;

    case BLIND:
        correction_left = 0;
        correction_right = 0;
        break;

    default:
        correction_left = -7;
        correction_right = -7;
        break;
    }

    correction_output = {correction_left,correction_right};
    // ble->print("L: ");
    // ble->println(correction_left);
    // ble->print("R: ");
    // ble->println(correction_right);

    return correction_output;
}

void pid_move_function(std::vector<int> correction){

    // ForwardLeft(base_speed + correction[0]);
    // ForwardRight(base_speed + correction[1]);


    if(PID_values[0] + base_speed > 0){
        ForwardLeft(base_speed + PID_values[0]);
    }
    else if(PID_values[0] + base_speed < 0){
        BackwardLeft(base_speed + PID_values[0]);
    }
    else if(PID_values[0] == 0){
        ForwardBoth(0);
    }

    if(PID_values[1] + base_speed > 0 && PID_values[0] + base_speed != 0){
        ForwardRight(base_speed + PID_values[1]);
    }
    else if(PID_values[1] + base_speed < 0 && PID_values[0] + base_speed != 0){
        BackwardRight(base_speed + PID_values[1]);
    }
    else if(PID_values[1] == 0){
        ForwardBoth(0);
    }

}

std::vector<bool> find_walls(){
    std::vector<bool> wallsVec = {false, false, false, false};
    // Left Wall Sensors
    if(Distance_Sensor[0] > 300 || Distance_Sensor[1] > 300){ // 100 und 40
        wallsVec[3] = true;
    }
    else{
        wallsVec[3] = false;
    }
    
    // Front Wall
    if(Distance_Sensor[2] > 300 || Distance_Sensor[3] > 300){
        wallsVec[0] = true;
    }
    else{
        wallsVec[0] = false;
    }

    // Right Wall
    if(Distance_Sensor[4] > 300 || Distance_Sensor[5] > 300){ //70 und 150
        wallsVec[1] = true;
    }
    else{
        wallsVec[1] = false;
    }

    return wallsVec;
}

int determine_PID_case(){
    std::vector<bool> wallsVec = find_walls();
    if(wallsVec[1] == true && wallsVec[3] == true){
        ble->println("b");
        return X_ERROR;
    }
    // else if(wallsVec[0] == true){
    //     ble->println("y");
    //     return Y_ERROR;
    // }
    else if(wallsVec[3] == true && wallsVec[1] == false){
        ble->println("l");
        return X_ERROR_LEFT_WALL_ONLY;
    }
    else if(wallsVec[3] == false && wallsVec[1] == true){
        ble->println("r");
        return X_ERROR_RIGHT_WALL_ONLY;
    }
    else if(wallsVec[1] == false && wallsVec[3] == false){
        ble->println("n");
        return BLIND;
    }
    return 0; 
}


void PID_Test(){
    digitalWrite(MOTOR_ENABLE, LOW);
    while(1){
        delay(1);
        //debug_print();

        pid_move_function(calc_correction(X_ERROR_LEFT_WALL_ONLY));

        if(encoderTurned){
            // || Distance_Sensor[2] > 500 || Distance_Sensor[3] > 500
            ForwardBoth(0);
            encoderTurned=false;
            digitalWrite(MOTOR_ENABLE, HIGH);
            break;
        }
    }
}


void reset_PID_values(){
    PID_values = {0,0};
    integral = 0;
    differential = 0;
    proportional = 0;
}