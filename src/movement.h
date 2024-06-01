#pragma once
#include <stdlib.h>
#include <cmath> // abs()
#include <vector>
#include <robin.h>
#include "Setup\Setup.h"

#define DUTY_SLOW 20
#define DUTY_SLOW_ROTATION 10
#define DUTY_FAST 60
#define TICKS_INNER_WHEEL 8600
#define TICKS_OUTER_WHEEL 18105

// declarations
// int duty_cycle; // fraction of max speed (0-100)
int duty_interval; // for stepwise acceleration
int duty_L;
int duty_R;
int distance_traveled_L = 0; // TODO: evtl float
int distance_traveled_R = 0; // TODO: evtl float
int avg_distance_traveled = 0; // TODO: evtl float
enum state{slow, fast};

// placeholder for encoder values
int encoder_L = 0;
int encoder_R = 0;

// tick variables: [distance / (2,2cm * pi)] * 5 * 2048 Ticks
int tick_forward = 26668 * 2;
int tick_start = 6889;
int tick_rotate = 7460;
int tick_accelerate = 13355;

// placeholder for sensor values
int sensor_LA, sensor_LI, sensor_LV, sensor_RV, sensor_RA, sensor_RI;
int threshhold; // for wall detection missing

std::array<bool, 4> current_walls = {false, false, false, false}; // [N, E, S, W]

void precompute_duty_intervals(){
    duty_interval = (DUTY_FAST - DUTY_SLOW) / 4;
}

void compute_avg_distance_traveled(){
    avg_distance_traveled = (abs(distance_traveled_L) + abs(distance_traveled_R)) * 2;
}

void reset_distance_traveled(){
    distance_traveled_L = 0;
    distance_traveled_R = 0;
}


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

// Low Level Function 
// SET SPEED OF MOTORS DIRECTLY VIA DUTY CYCLE
// TODO: include PID values
void move_actual(int duty_cycle){
    // std::vector<int> PIDvec = pid.calcOutput(/*sensor values*/);
    // duty_L = duty_cycle + PIDvec[0];
    // duty_R = duty_cycle + PIDvec[1];
    ForwardBoth(duty_cycle); // set actual motor speed
}

// Middle layer function to move forwards
// Stay in this function until we have reached a multiple of one tick forward (square center to square center)
void move_forward_middle_level(int duty_cycle, float squares = 1.0){
    int desired_distance = static_cast<int>(round(squares * tick_forward));

    // Until we have reached the desired distance, keep moving at given duty cycle
    ble->println("Moving forward");
    ble->println(desired_distance);
    ble->println(avg_distance_traveled);
    move_actual(duty_cycle);
    encoder_L = 0; 
    encoder_R = 0;
    while (avg_distance_traveled < desired_distance){
        delay(1); // TODO: improve this
    }
    ble->println("Reached destination");
    ble->print("Avg distance traveled to this point: ");
    ble->println(avg_distance_traveled);

    // stops to scan when mapping
    // if (duty_cycle == DUTY_SLOW){
    //     duty_cycle = 0;
    //     move_actual(duty_cycle);
    //     scan_walls();
    // }
    // reset_distance_traveled();
}

// mapping movement

void go_to_start(int duty_cycle){
    while(avg_distance_traveled < tick_start){
        move_actual(duty_cycle);
    }
    reset_distance_traveled();
}

void rotate_left(){
    // while we have not turned a quarter of a circle
    while(avg_distance_traveled < tick_rotate){
        ForwardRight(DUTY_SLOW_ROTATION);  // continue moving wheels in opposite directions
        BackwardLeft(DUTY_SLOW_ROTATION);
    }
    reset_distance_traveled();
}

void rotate_right(){
    // while we have not turned a quarter of a circle
    while(avg_distance_traveled < tick_rotate){
        BackwardRight(DUTY_SLOW_ROTATION); // continue moving wheels in opposite directions
        ForwardLeft(DUTY_SLOW_ROTATION);
    }
    reset_distance_traveled();
}

void turn_around(){
    while(avg_distance_traveled < (tick_rotate * 2)){
        // basically turning left left two times 
        ForwardRight(DUTY_SLOW_ROTATION); 
        BackwardLeft(DUTY_SLOW_ROTATION); 
    }
    reset_distance_traveled();
}

// fast-run movement

void left_curve(int duty_cycle){
    duty_L = round(duty_cycle * 0.475); // curve speed ratio
    duty_R = duty_cycle;
    while(distance_traveled_L < TICKS_INNER_WHEEL || distance_traveled_R < TICKS_OUTER_WHEEL){
        ForwardLeft(duty_L);
        ForwardRight(duty_R);
    }
    reset_distance_traveled();
}

void right_curve(int duty_cycle){
    duty_L = duty_cycle;
    duty_R = round(duty_cycle * 0.475); // curve speed ratio
    while(distance_traveled_L < TICKS_OUTER_WHEEL || distance_traveled_R < TICKS_INNER_WHEEL){
        ForwardRight(duty_R);
        ForwardLeft(duty_L);
    }
    reset_distance_traveled();
}

void accelerate(){ 
    while(avg_distance_traveled < tick_accelerate){
        int duty_cycle = DUTY_SLOW;
        for(int n = 1; n < 4; n++){
            duty_cycle += duty_interval;      // stepwise acceleration
            int start_time = millis();
            move_actual(duty_cycle);
            delay(50); // TODO: improve this
        }
        move_actual(DUTY_FAST);
    }
    reset_distance_traveled();
}

void decelerate(){
    while(avg_distance_traveled < tick_accelerate){
        int duty_cycle = DUTY_FAST;
        for(int n = 1; n < 4; n++){
             duty_cycle -= duty_interval;      // stepwise deceleration
            int start_time = millis();
            move_actual(duty_cycle);
            delay(50); // TODO: improve this
        }
        move_actual(DUTY_SLOW);
    }
    reset_distance_traveled();
}

// void calc_next_move(std::vector<std::string> moves, int state){
//     if(state == slow && moves[2] == "Forward"){
//         accelerate();
//         state = fast;
//     } else if(state == slow && moves[2] == "Turn"){
//         move_forward_low_level(30);
//         state = slow;
//     } else if(state == fast && moves[2] == "Forward"){
//         move_forward_low_level(70);
//         state = slow;    
//     } else if(state == fast && moves[2] == "Turn"){
//         decelerate();
//         state = slow;    
//     } else if(moves[0] = "Turn" && moves[2] == "Turn"){
//         state = slow;    
//     }
// }