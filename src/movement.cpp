#pragma once
#include <stdlib.h>
#include <cmath> // abs()
#include <vector>

// declarations
int duty_cycle; // fraction of max speed (0-100)
int duty_L;
int duty_R;
int distance_traveled_L = 0;
int distance_traveled_R = 0;
int avg_distance_traveled = (abs(distance_traveled_L) + abs(distance_traveled_R)) / 2;
enum state{slow, fast};

// placeholder for encoder values
int encoder_L = 0;
int encoder_R = 0;

// tick variables: [distance / (2,2cm * pi)] * 2048 Ticks
int tick_forward = 5334;
int tick_start = 1378;
int tick_rotate = 1492;
int tick_accelerate = 2671;

// placeholder for sensor values
int sensor_LA, sensor_LI, sensor_LV, sensor_RV, sensor_RA, sensor_RI;
int threshhold; // for wall detection missing

// running wall-array overwrites values of each cell every few ticks
void scan_walls(){
    if (sensor_LA > threshhold && sensor_LI > threshhold){
        // left wall = true
    }
    if (sensor_LV > threshhold && sensor_RV > threshhold){
        // front wall = true
    }
    if (sensor_RA > threshhold && sensor_RI > threshhold){
        // right wall = true
    }
}

void move_actual(int duty_cycle){
    std::vector<int> PIDvec = pid.calcOutput(/*sensor values*/);
    duty_L = duty_cycle + PIDvec[0];
    duty_R = duty_cycle + PIDvec[1];
}

void move_forward(int duty_cycle){
    while (avg_distance_traveled < tick_forward){

        distance_traveled_L += encoder_L;
        distance_traveled_R += encoder_R;
        move_actual(duty_cycle);
    }

    // stops to scan when mapping
    if (duty_cycle < 50){
        duty_cycle = 0;
        move_actual(duty_cycle);
        scan_walls();
    }

    distance_traveled_L = 0;
    distance_traveled_R = 0;
}

// mapping movement

void go_to_start(){
    while(avg_distance_traveled < tick_start){

        distance_traveled_L += encoder_L;
        distance_traveled_R += encoder_R;

        duty_cycle = 30;
        move_actual(duty_cycle);
    }
    duty_cycle = 0;
    move_actual(duty_cycle);
    scan_walls();

    distance_traveled_L = 0;
    distance_traveled_R = 0;
}

void rotate_left(){
    while(avg_distance_traveled < tick_rotate){

        distance_traveled_L += encoder_L;
        distance_traveled_R += encoder_R;

        duty_L = -30;
        duty_R = 30;
    }
    distance_traveled_L = 0;
    distance_traveled_R = 0;
}

void rotate_right(){
    while(avg_distance_traveled < tick_rotate){

        distance_traveled_L += encoder_L;
        distance_traveled_R += encoder_R;

        duty_L = 30;
        duty_R = -30;
    }
    distance_traveled_L = 0;
    distance_traveled_R = 0;
}

void turn_around(){
    while(avg_distance_traveled < (tick_rotate * 2)){

        distance_traveled_L += encoder_L;
        distance_traveled_R += encoder_R;

        duty_L = -30;
        duty_R = 30;
    }
    distance_traveled_L = 0;
    distance_traveled_R = 0;
}

// fast-run movement

void left_curve(int duty_cycle){
    while(distance_traveled_L < 1720 || distance_traveled_R < 3621){
        
        distance_traveled_L += encoder_L;
        distance_traveled_R += encoder_R;

        duty_L = duty_cycle * 48; // 47.5 rounded up
        duty_R = duty_cycle;
    }
    distance_traveled_L = 0;
    distance_traveled_R = 0;
}

void right_curve(int duty_cycle){
    while(distance_traveled_L < 3621 || distance_traveled_R < 1720){

        distance_traveled_L += encoder_L;
        distance_traveled_R += encoder_R;

        duty_L = duty_cycle;
        duty_R = duty_cycle * 48; // 47.5 rounded up
    }
    distance_traveled_L = 0;
    distance_traveled_R = 0;
}

void accelerate(){
    while(avg_distance_traveled < tick_accelerate){

        for(int n = 0; n < 5; n++){

            duty_cycle = 30 + n * 10;      // stepwise acceleration
            start_time = millis();
            while(millis() < (start_time + 50)){
                move_actual(duty_cycle);
                distance_traveled_L += encoder_L;
                distance_traveled_R += encoder_R;
            }
            distance_traveled_L += encoder_L;
            distance_traveled_R += encoder_R;
        }
    }
    distance_traveled_L = 0;
    distance_traveled_R = 0;
}

void decelerate(){
    while(avg_distance_traveled < tick_accelerate){

        for(int n = 0; n < 5; n++){

            duty_cycle = 70 - n * 10;      // stepwise deceleration
            start_time = millis();
            while(millis() < (start_time + 50)){
                move_actual(duty_cycle);
                distance_traveled_L += encoder_L;
                distance_traveled_R += encoder_R;
            }
            distance_traveled_L += encoder_L;
            distance_traveled_R += encoder_R;
        }
    }
    distance_traveled_L = 0;
    distance_traveled_R = 0;
}

void calc_next_move(std::vector<std::string> moves, int state){
    if(state == slow && moves[2] == "Forward"){
        accelerate();
        state = fast;
    } else if(state == slow && moves[2] == "Turn"){
        move_forward(30);
        state = slow;
    } else if(state == fast && moves[2] == "Forward"){
        move_forward(70);
        state = slow;    
    } else if(state == fast && moves[2] == "Turn"){
        decelerate();
        state = slow;    
    } else if(moves[0] = "Turn" && moves[2] == "Turn"){
        state = slow;    
    }
}