#pragma once
#include "Setup\Setup.h"
#include <cmath>

//Variable definitons
double last_time=0;
double dt, kp_default=2, ki_default=0, kd_default=0.001;
double proportional, integral, differential;
signed int previous=0;
int default_base_speed = 100;
int default_max_correction_speed_x = 150;
int max_correction_speed = 150;
int default_max_correction_speed_y = 20;
int current_state;
int toggle_drive;
int LAST_CASE_PID = -1;
bool SET_PID_MANUALLY = false;
bool PID_ENABLED = true;
std::vector<int> avg_PID_values(2, 0);
std::vector<int> PID_values_encoder = {0,0};
std::vector<int> remapped_error(ENUM_END,0); //, remapped_error_encoder = 0;
int CURRENT_CASE_PID = X_ERROR_LEFT_WALL_ONLY;
std::vector<int> NeutralSensorValues {532, 470, 803, 678, 672, 475, 1423};
std::vector<int> ErrorSensorsVec {0,0,0,0,0,0};

//toggle PID on and off
void enable_PID(){
    PID_ENABLED = true;
}

void disable_PID(){
    PID_ENABLED = false;
}


// Function to print the sensor values to the bluetooth module
 void debug_print(){
    for(int i=0;i<6;i++){
            ble->print(i);
            ble->print(": ");
            ble->println(Distance_Sensor[i]);
            if(i==1){
                i+=2;
            }
        }
    ble->println("");
 }

 // Function to calculate the time difference between two calls -> delta t
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
    }
}

//adjust possible correction according to speed
double give_percent(double value, int istart = 0, int istop = 1) {
    return -100 + 200 * ((value - istart) / (istop - istart));
}

//Calculate error for PID (dependent on current case)
int calcError(int PID_case){
    calc_Sensor_Errors();
    int error, desired_distance_L, desired_distance_R;
    switch (PID_case)
    {

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

    case X_ERROR_ENCODER_BASED:
        error = distance_traveled_L_PID - distance_traveled_R_PID;
        break;

    default:
        error = 0;
        break;
    }
    return error;
}

//Apply PID to given error
int applyPID(signed int error, double kp = 0.5 , double ki = 0, double kd = 0){

    proportional = error;
    // cap the integral value
    if(abs(integral) < 100000){
        integral += error * dt;
    }
    // calculate the PID values
    differential = (error-previous)/dt;
    double output = kp * proportional + ki*integral + kd*differential;
    previous = error;
    int output_G_int = static_cast<int>(round(output));
    return output_G_int;
}

//cap max correction speed
int cap_output(int output_G, int max_correction_speed = 15){
    if(output_G > max_correction_speed){
        output_G = max_correction_speed;
    }
    else if(output_G < -max_correction_speed){
        output_G = -max_correction_speed;
    }
    return output_G;
}

//Calculate correction vector
std::vector<int> calc_correction(int PID_case){
    dt = calc_dt();

    signed int correction_left, correction_right;
    std::vector<signed int> correction_output;
    signed int output_G;

    switch (PID_case)
    {

    case X_ERROR:
        max_correction_speed = default_max_correction_speed_x;
        max_values_lower_boundary[X_ERROR] = max_values_left[X_ERROR];
        max_values_upper_boundary[X_ERROR] = max_values_right[X_ERROR];
        remapped_error[X_ERROR] = give_percent(calcError(X_ERROR),max_values_lower_boundary[X_ERROR],max_values_upper_boundary[X_ERROR]); //-7;
        output_G = applyPID(remapped_error[X_ERROR], 0.35, 0.0003, 0.001); //<-- Case Specific kp,ki,kd-values can be defined here, +-5000 can later be switch out with calibration values
        output_G = cap_output(output_G, max_correction_speed); //1.7 0 0.5
        correction_left = -output_G;
        correction_right = output_G;
        break;

    case Y_ERROR:
        max_correction_speed = default_max_correction_speed_y;
        max_values_lower_boundary[Y_ERROR] = max_values_front[Y_ERROR];
        max_values_upper_boundary[Y_ERROR] = max_values_back[Y_ERROR];
        remapped_error[Y_ERROR] = give_percent(calcError(Y_ERROR),max_values_lower_boundary[Y_ERROR],max_values_upper_boundary[Y_ERROR]);
        output_G = applyPID(remapped_error[Y_ERROR], 2, 0, 0); // 0.8,0.0005,0.01
        // ble->println(integral);
        output_G = cap_output(output_G, max_correction_speed);
        correction_left = -output_G;
        correction_right = -output_G;
        break;

    case X_ERROR_LEFT_WALL_ONLY:
        max_correction_speed = default_max_correction_speed_x;
        max_values_upper_boundary[X_ERROR_LEFT_WALL_ONLY] = max_values_left[X_ERROR_LEFT_WALL_ONLY];
        max_values_lower_boundary[X_ERROR_LEFT_WALL_ONLY] = max_values_right[X_ERROR_LEFT_WALL_ONLY]-100;
        remapped_error[X_ERROR_LEFT_WALL_ONLY] = give_percent(calcError(X_ERROR_LEFT_WALL_ONLY),max_values_lower_boundary[X_ERROR_LEFT_WALL_ONLY],max_values_upper_boundary[X_ERROR_LEFT_WALL_ONLY]); //+59;
        output_G = applyPID(remapped_error[X_ERROR_LEFT_WALL_ONLY], 0.35, 0.0003, 0.001); // 12, 0.0001, 0.01
        output_G = cap_output(output_G, max_correction_speed);
        correction_left = output_G;
        correction_right = -output_G;
        break;

    case X_ERROR_RIGHT_WALL_ONLY:
        max_correction_speed = default_max_correction_speed_x;
        max_values_upper_boundary[X_ERROR_RIGHT_WALL_ONLY] = max_values_left[X_ERROR_RIGHT_WALL_ONLY];
        max_values_lower_boundary[X_ERROR_RIGHT_WALL_ONLY] = max_values_right[X_ERROR_RIGHT_WALL_ONLY]-100;
        remapped_error[X_ERROR_RIGHT_WALL_ONLY] = give_percent(calcError(X_ERROR_RIGHT_WALL_ONLY),max_values_lower_boundary[X_ERROR_RIGHT_WALL_ONLY],max_values_upper_boundary[X_ERROR_RIGHT_WALL_ONLY]); //-68;
        output_G = applyPID(remapped_error[X_ERROR_RIGHT_WALL_ONLY], 0.35, 0.0003, 0.001);
        output_G = cap_output(output_G, max_correction_speed);
        correction_left = output_G;
        correction_right = -output_G;
        break;

    case BLIND: //<-- Can be deleted an replaced by X_ERROR_ENCODER BASED
        remapped_error[BLIND] = 0;
        correction_left = avg_PID_values[0]; // using average PID values to just drive straight
        correction_right = avg_PID_values[1];
        break;

    case TRANSITION:
        remapped_error[TRANSITION] = 0;
        correction_left = avg_PID_values[0]; // using average PID values to just drive straight
        correction_right = avg_PID_values[1];
        break;

     case X_ERROR_ENCODER_BASED:
        max_correction_speed = default_max_correction_speed_x;
        remapped_error[X_ERROR_ENCODER_BASED] = give_percent(calcError(X_ERROR_ENCODER_BASED),-40960,40960);
        output_G = applyPID(remapped_error[X_ERROR_ENCODER_BASED], 0.5, 0, 0); //<-- Case Specific kp,ki,kd-values can be defined here, +-5000 can later be switch out with calibration values
        output_G = cap_output(output_G, max_correction_speed);
        correction_left = -output_G;
        correction_right = output_G;
        break;

    default:
        correction_left = avg_PID_values[0];
        correction_right = avg_PID_values[1];
        break;
    }

    correction_output = {correction_left,correction_right};
    return correction_output;
}

//Determine where Walls are
std::vector<bool> find_walls(){
    std::vector<bool> wallsVec = {false, false, false, false};
    // Left Wall Sensors
    if(Distance_Sensor[0] > MinSensorValues[0] ){ // 100 und 40
        wallsVec[3] = true;
    }
    // Front Wall
    if(Distance_Sensor[6] > MinSensorValues[6]){ // 70 und 150
        wallsVec[0] = true;
    }

    // Right Wall
    if(Distance_Sensor[5] > MinSensorValues[5]) { //70 und 150
        wallsVec[1] = true;
    }

    return wallsVec;
}

bool front_wall_detected(){
    return Distance_Sensor[6] > MinSensorValues[6];
}

bool side_walls_disappearing(){
    // delta_sensors > typical sensor values - min sensor values when there is still a wall
    int comp_left = Last_Distance_Sensor[0] - Distance_Sensor[0];
    int comp_right = Last_Distance_Sensor[5] - Distance_Sensor[5];
    // ble->println("L :" + comp_left);
    ble->println(comp_right);
    return Last_Distance_Sensor[0] - Distance_Sensor[0] > 50 || Last_Distance_Sensor[5] - Distance_Sensor[5] > 50;
}

int determine_PID_case(){
    std::vector<bool> walls_compare_threshold(7, false);
    for (int i = 0; i < 6; i++){
        walls_compare_threshold[i] = Distance_Sensor[i] > (NeutralSensorValues[i] * 0.3); // threshold of 50% of the neutral value
    }
    if (walls_compare_threshold[0] && walls_compare_threshold[5] && walls_compare_threshold[1] && walls_compare_threshold[4] ){
        return X_ERROR;
    } else if (walls_compare_threshold[0] && walls_compare_threshold[1] && (!walls_compare_threshold[4] || !walls_compare_threshold[5])){
        return X_ERROR_LEFT_WALL_ONLY;
    } else if (walls_compare_threshold[4] && walls_compare_threshold[5] && (!walls_compare_threshold[0] || !walls_compare_threshold[1])){
        return X_ERROR_RIGHT_WALL_ONLY;
    } else if (!walls_compare_threshold[0] && !walls_compare_threshold[1] && !walls_compare_threshold[4] && !walls_compare_threshold[5]){
        return X_ERROR_ENCODER_BASED;
    } else {
        return X_ERROR_ENCODER_BASED;
    }
    return 10; // just default but will not be reached
}


void pid_move_function(int base_speed){
    std::vector<int> old_pid = PID_values;
    int counter = 0;
    while(1){
        delay(8);
        old_pid = PID_values;

        if(encoderTurned){
            ForwardBoth(0);
            encoderTurned=false;
            break;
        }

        if(PID_values[0]+avg_PID_values[0] > 1){
            ForwardLeft(base_speed + avg_PID_values[0] + PID_values[0]);
            ForwardRight(base_speed + avg_PID_values[1] + PID_values[0]);
        }
        else if(PID_values[0]+avg_PID_values[0] < -1){
            BackwardLeft(base_speed - avg_PID_values[0] - PID_values[0]);
            BackwardRight(base_speed - avg_PID_values[1] - PID_values[0]);
        }
        else if(abs(PID_values[0]) + base_speed <= 51){
            ForwardLeft(0);
            ForwardRight(0);
            counter++; 
            if (counter > 20) break;
        }
    }
    return;
}

// Function to calculate the average PID values
void calc_average_PID_values() {
  static int counter = 0;
  counter++;

  avg_PID_values[0] += PID_values[0]; 
  avg_PID_values[1] += PID_values[1];

  avg_PID_values[0] = static_cast<int>(std::round(static_cast<double>(avg_PID_values[0]) / counter));
  avg_PID_values[1] = static_cast<int>(std::round(static_cast<double>(avg_PID_values[1]) / counter));
}

// Function to reset the PID values
void reset_PID_values(){
    PID_values = {0,0};
    integral = 0;
    differential = 0;
    proportional = 0;
}

// Function that is called if there is a wall in front of the robot such that we can use the front wall to recalibrate position
void recalibrate_front_wall(){
    // if the front wall is detected, we can use the front wall to recalibrate the position of the robot
    if(Distance_Sensor[6] > NeutralSensorValues[6] * 0.5 || Distance_Sensor[2] > NeutralSensorValues[2]*0.5 || Distance_Sensor[3] > NeutralSensorValues[3]*0.5){
        SET_PID_MANUALLY = true;
        CURRENT_CASE_PID = Y_ERROR;
        delay(800);
        reset_PID_values();
        pid_move_function(50);
        delay(800);
        SET_PID_MANUALLY = false;
        reset_PID_values();
    }
}

void encoder_based_move_function(int base_speed){
    reset_distance_traveled();

    while(1){
        delay(8);

        if(encoderTurned){
            ForwardBoth(0);
            encoderTurned=false;
            break;
        }

        std::vector<int> PID_values_encoder = calc_correction(X_ERROR_ENCODER_BASED);
        int both_zero = 0;
        ble->println(PID_values_encoder[0]);
        ble->println(PID_values_encoder[1]);
        if(default_base_speed + PID_values_encoder[0] > 50){
            ForwardLeft(default_base_speed + PID_values_encoder[0]);
            both_zero = 0;
            //ForwardRight(default_base_speed + PID_values_encoder[1]);
        }
        else if(default_base_speed + PID_values_encoder[0] < 50){
            BackwardLeft(default_base_speed - PID_values_encoder[0]);
            both_zero = 0;
            //BackwardRight(default_base_speed - avg_PID_values[1] - PID_values[0]);
        }
        else{
            ForwardBoth(0);
            both_zero = 1;
        }

        if(default_base_speed + PID_values_encoder[1] > 50 && both_zero == 0){
            ForwardRight(default_base_speed + PID_values_encoder[1]);
            //ForwardRight(default_base_speed + PID_values_encoder[1]);
        }
        else if(default_base_speed + PID_values_encoder[1] < 50 && both_zero == 0){
            BackwardRight(default_base_speed - PID_values_encoder[1]);
            //BackwardRight(default_base_speed - avg_PID_values[1] - PID_values[0]);
        }
        else{
            ForwardBoth(0);
        }
    }
    return;
}