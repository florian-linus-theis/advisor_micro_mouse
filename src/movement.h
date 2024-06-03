#pragma once
#include <stdlib.h>
#include <cmath> // abs()
#include <vector>
#include <robin.h>
#include "Setup\Setup.h"


int precompute_duty_intervals();

// declarations
// int duty_cycle; // fraction of max speed (0-100)
int duty_interval = precompute_duty_intervals(); // for stepwise acceleration
int duty_L = 0;
int duty_R = 0;
int distance_traveled_L = 0; // TODO: evtl float
int distance_traveled_R = 0; // TODO: evtl float
int avg_distance_traveled = 0; // TODO: evtl float
int current_speed = 0;
int current_duty_cycle = 0;
enum state{slow, fast};

// placeholder for encoder values
int encoder_L = 0;
int encoder_R = 0;

// tick variables: [distance / (2,2cm * pi)] * 5 * 2048 Ticks
int tick_forward = 26668 * 4;
int tick_start = 6889 * 4;
int tick_rotate = 7460 * 4;
int tick_accelerate = 13355 * 4;

// placeholder for sensor values
int sensor_LA, sensor_LI, sensor_LV, sensor_RV, sensor_RA, sensor_RI;
int threshhold; // for wall detection missing

std::array<bool, 4> current_walls = {false, false, false, false}; // [N, E, S, W]

int precompute_duty_intervals(){
    return (DUTY_FAST - DUTY_SLOW) / 4;
}

void compute_avg_distance_traveled(){
    avg_distance_traveled = (abs(distance_traveled_L) + abs(distance_traveled_R)) / 2;
}

void reset_distance_traveled(){
    distance_traveled_L = 0;
    distance_traveled_R = 0;
}


// convert speed to duty cycle
double duty_to_speed(int duty_cycle){
    return  SPEED_TO_DUTY_FACTOR * duty_cycle;
}

// convert duty cycle to speed
int speed_to_duty(double speed){
    return static_cast<int>(speed / SPEED_TO_DUTY_FACTOR);
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
    //ForwardBoth(duty_cycle); // set actual motor speed
    int correction = duty_cycle * (4.0/ 150.0);
    ForwardLeft(duty_cycle + correction);
    ForwardRight(duty_cycle);
}

void stop(){
    ForwardLeft(0);
    ForwardRight(0);
    delay(1000);
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
    reset_distance_traveled();
    while (avg_distance_traveled < desired_distance){
        delay(1); // TODO: improve this
    }
    ble->println("Reached destination");
    ble->print("Avg distance traveled to this point: ");
    ble->println(avg_distance_traveled);

    // stops to scan when mapping
    if (duty_cycle == DUTY_SLOW){
        duty_cycle = 0;
        stop(); // stop motors
    }
    stop();
    reset_distance_traveled();
}

int calc_braking_distance(int end_duty_cycle){
    // Check if the desired duty cycle is higher than the current duty cycle
    if (end_duty_cycle > current_duty_cycle) {
        return -1; // No braking distance if the desired duty cycle is higher
    }
    // If the end duty cycle is the same as the current, no braking distance is needed
    if (end_duty_cycle == current_duty_cycle) {
        return 0;
    }

    // Calculate the scaling factors based on the squares of the duty cycles
    double start_duty_cycle_square = static_cast<double>(current_duty_cycle * current_duty_cycle);
    double end_duty_cycle_square = static_cast<double>(end_duty_cycle * end_duty_cycle);
    double duty_slow_square = static_cast<double>(DUTY_SLOW * DUTY_SLOW);

    // Calculate the braking distance using the known distance and the scaling factors
    double braking_distance = KNOWN_BRAKE_DIST_AT_DUTY_SLOW * (start_duty_cycle_square - end_duty_cycle_square) / duty_slow_square;

    return static_cast<int>(std::round(braking_distance));
}

void accelerate_different(int desired_duty_cycle){
    // Function to accelerate the motor to roughly 2/3 of the desired duty cycle (to avoid overshooting) -> after 7 calls we should be at the desired duty cycle
    int target_duty_cycle = current_duty_cycle + static_cast<int>(std::round((desired_duty_cycle - current_duty_cycle) * 2.0 / 3.0));
    ForwardBoth(target_duty_cycle); // Update motors to the new duty cycle
    ble->print("Accelerating to: ");
    ble->println(target_duty_cycle);
    current_duty_cycle = target_duty_cycle; // update current duty cycle
}

void decelerate_different(int end_duty_cycle, int remaining_distance_ticks){
    // If we are close to the end, stop the motors
    if (remaining_distance_ticks <= DISTANCE_DUTY_MIN_TO_ZERO && end_duty_cycle == 0) {
        ble->println("Stopping motors");
        stop();
    }
    // Decelerate to 2/3 of the difference between current and desired duty cycles
    int target_duty_cycle = current_duty_cycle - static_cast<int>(std::round((current_duty_cycle - end_duty_cycle) * 2.0 / 3.0));
    // ensuring that we do not go below the minimum duty cycle so that we continue cruising
    if (target_duty_cycle < MINIMUM_DUTY) {
        target_duty_cycle = MINIMUM_DUTY;
    }
    ForwardBoth(target_duty_cycle);
    ble->print("Decelerating to: ");
    ble->println(target_duty_cycle);
    current_duty_cycle = target_duty_cycle;
}

void printer_debugger(int current_speed, int average_distance_travelled, int distance_remaining, int braking_distance){
    ble->print("Current speed: ");
    ble->println(current_speed);
    ble->print("Last distance traveled: ");
    ble->println(average_distance_travelled);
    ble->print("Distance remaining: ");
    ble->println(distance_remaining);
    ble->print("Braking distance: ");
    ble->println(braking_distance);
}


void move_forward_different(int desired_max_duty_cycle, int end_duty_cycle, float squares){
    // first reset distances
    reset_distance_traveled(); // perhaps can be deleted because we want to account for having driven too far since the last time we reset
    int desired_distance = static_cast<int>(round(squares * tick_forward));
    int last_distance_traveled = 0;	
    while(avg_distance_traveled < desired_distance){
        if (last_distance_traveled == avg_distance_traveled) continue; // if the systick has not updated our values, do not update pwm values etc.
        int distance_remaining = desired_distance - avg_distance_traveled;
        int braking_distance = calc_braking_distance(end_duty_cycle); 
        printer_debugger(current_speed, avg_distance_traveled, distance_remaining, braking_distance); // debugging
        if (distance_remaining > braking_distance){
            if (current_speed < duty_to_speed(desired_max_duty_cycle)){
                accelerate_different(desired_max_duty_cycle);
            }
            // else if (current_speed > desired_max_speed){
        } else {
            decelerate_different(end_duty_cycle, distance_remaining);
        }
        last_distance_traveled = avg_distance_traveled; // update the last distance traveled
    }
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
    int correction = static_cast<int>(static_cast<double>(duty_cycle) * (4.0/ 150.0));
    duty_L = duty_cycle + correction;
    duty_R = static_cast<int>((static_cast<double>(duty_cycle) * 0.475)); // curve speed ratio
    ForwardRight(duty_R);
    ForwardLeft(duty_L);
    while(distance_traveled_L < TICKS_OUTER_WHEEL || distance_traveled_R < TICKS_INNER_WHEEL){
        delay(1);
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

void decelerate(int start_duty_cycle, int end_duty_cycle){

    while(avg_distance_traveled < tick_accelerate){
        int duty_cycle = start_duty_cycle;
        for(int n = 1; n < 4; n++){
            duty_cycle -= duty_interval;      // stepwise deceleration
            int start_time = millis();
            move_actual(duty_cycle);
            delay(4); // TODO: improve this
        }
        move_actual(DUTY_SLOW);
    }
    reset_distance_traveled();
}
