#pragma once
#include "Setup\Setup.h"
#include <cmath>

double last_time=0;
double dt, kp_default=2, ki_default=0, kd_default=0.001;
double proportional, integral, differential;
signed int previous=0;
int base_speed = 100;
int default_max_correction_speed = 130, max_correction_speed;
int current_state;
int toggle_drive;

enum{
    ROT_ERROR = 0,
    X_ERROR = 1,
    Y_ERROR = 2,
    CURVE_LEFT_ERROR = 3,
    CURVE_RIGHT_ERROR = 4,
    X_ERROR_LEFT_WALL_ONLY = 5,
    X_ERROR_RIGHT_WALL_ONLY = 6,
    BLIND = 7
};

//std::vector<int> NeutralSensorValues {150,78,330,230,60,144};
std::vector<int> NeutralSensorValues {237, 110, 340, 407, 510, 171};
std::vector<signed int> ErrorSensorsVec {0,0,0,0,0,0};

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
	//return -100 + (100 - (-100)) * ((value - (-5000)) / (5000 - (-5000)));
    return -100 + 200 * ((value +5000) / 10000);
}

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

    case Y_ERROR:
        error = (ErrorSensorsVec[2]+ErrorSensorsVec[3]);
        error = error/2;
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
        error = error/2;
        break;

    case X_ERROR_RIGHT_WALL_ONLY:
        error = -(ErrorSensorsVec[4]+ErrorSensorsVec[5]);
        error = error/2;
        break;

    default:
        error = 0;
        break;
    }

    return error;
}

int applyPID(signed int error, double kp = 3, double ki = 0.0001, double kd = 0.002){
    error = give_percent(error);
    proportional = error;
    if(abs(integral) < 100000){
        integral += error * dt;
    }
    double output;
    // if((abs(integral*ki) < 7 && ki > 0.001) || abs(error) < 5){
    //     output = 0;
    //     toggle_drive = 0;

    // }
    // else{
    differential = (error-previous)/dt;
    output = kp * proportional + ki*integral + kd*differential;
    // }
    // toggle_drive = 1;
    //ble->println(output);
    previous = error;
    int output_G_int = static_cast<int>(round(output));
    return output_G_int;
}

//cap max correction speed
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
        output_G = applyPID(calcError(X_ERROR),4,0,0.01); //<-- Case Specific kp,ki,kd-values can be defined here
        output_G = cap_output(output_G,max_correction_speed);
        correction_left = -output_G;
        correction_right = output_G;
        break;

    case Y_ERROR:
        max_correction_speed = default_max_correction_speed; 
        output_G = applyPID(calcError(Y_ERROR),2,0.0001,0.01);
        output_G = cap_output(output_G,max_correction_speed);
        correction_left = -output_G;
        correction_right = -output_G;
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
        output_G = applyPID(calcError(X_ERROR_LEFT_WALL_ONLY),12,0.0001,0.02);
        output_G = cap_output(output_G,max_correction_speed);
        correction_left = output_G;
        correction_right = -output_G;
        break;

    case X_ERROR_RIGHT_WALL_ONLY:
        max_correction_speed = default_max_correction_speed;
        output_G = applyPID(calcError(X_ERROR_RIGHT_WALL_ONLY),12,0.0001,0.02);
        output_G = cap_output(output_G,max_correction_speed);
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


    if(correction[0] + base_speed > 0){
        ForwardLeft(base_speed + correction[0]);
    }
    else if(correction[0] + base_speed < 0){
        BackwardLeft(base_speed + correction[0]);
    }
    else if(correction[0] == 0){
        ForwardBoth(0);
    }

    if(correction[1] + base_speed > 0 && correction[0] + base_speed != 0){
        ForwardRight(base_speed + correction[1]);
    }
    else if(correction[1] + base_speed < 0 && correction[0] + base_speed != 0){
        BackwardRight(base_speed + correction[1]);
    }
    else if(correction[1] == 0){
        ForwardBoth(0);
    }

}

std::vector<bool> find_walls(){
    std::vector<bool> wallsVec2 = {true,true,true};
    if(Distance_Sensor[0] > 150 || Distance_Sensor[1] > 150){ // 100 und 40
        wallsVec2[0] = true;
    }
    else{
        wallsVec2[0] = false;
    }

    if(Distance_Sensor[2] > 150 || Distance_Sensor[3] > 150){
        wallsVec2[1] = true;
    }
    else{
        wallsVec2[1] = false;
    }

    if(Distance_Sensor[4] > 300 || Distance_Sensor[5] > 300){ //70 und 150
        wallsVec2[2] = true;
    }
    else{
        wallsVec2[2] = false;
    }

    return wallsVec2;
}

std::vector<int> determine_state(){
    std::vector<bool> wallsVec = find_walls();
    if(wallsVec[0] == true && wallsVec[2] == true){
        ble->println("b");
        current_state = X_ERROR;
        return calc_correction(X_ERROR);
    }
    else if(wallsVec[1] == true){
        ble->println("y");
        current_state = Y_ERROR;
        return calc_correction(Y_ERROR);
    }
    else if(wallsVec[0] == true && wallsVec[2] == false){
        ble->println("l");
        current_state = X_ERROR_LEFT_WALL_ONLY;
        return calc_correction(X_ERROR_LEFT_WALL_ONLY);
    }
    else if(wallsVec[0] == false && wallsVec[2] == true){
        ble->println("r");
        current_state = X_ERROR_RIGHT_WALL_ONLY;
        return calc_correction(X_ERROR_RIGHT_WALL_ONLY);
    }
    else if(wallsVec[0] == false && wallsVec[2] == false){
        ble->println("n");
        current_state = BLIND;
        return calc_correction(BLIND);
    }
    return {0,0}; // default
}


void PID_Test(){
    while(1){

        //Enable Motors
        digitalWrite(MOTOR_ENABLE, LOW);

        //Calculate dt for Integral Component of PID
        dt = calc_dt();

        //Calculate deviation from NeutralSensorValues (Error = 0 --> Mous is perfectly centered and oriented)
        calc_Sensor_Errors();

        //Calculate corrections and set motors accordingly
        // std::vector<int> y_component = calc_correction(Y_ERROR);
        // std::vector<int> x_component = calc_correction(X_ERROR);
        // std::vector<int> xy_correction;
        // xy_correction[0] = x_component[0] + y_component[0];
        // xy_correction[1] = x_component[1] + y_component[1];

        // pid_move_function(xy_correction);

        debug_print();

        pid_move_function(calc_correction(X_ERROR_RIGHT_WALL_ONLY));
        
        delay(10);

        if(encoderTurned){
            // || Distance_Sensor[2] > 500 || Distance_Sensor[3] > 500
            ForwardBoth(0);
            encoderTurned=false;
            digitalWrite(MOTOR_ENABLE, HIGH);
            break;
        }
    }
}