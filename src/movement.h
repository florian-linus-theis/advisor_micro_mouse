#pragma once
#include <stdlib.h>
#include <cmath> // abs()
#include <vector>
#include <motors.h>
#include "Setup/Setup.h"

int precompute_duty_intervals();

// declarations
// int duty_cycle; // fraction of max speed (0-100)
int duty_interval = precompute_duty_intervals(); // for stepwise acceleration
int duty_L = 0;
int duty_R = 0;
volatile int distance_traveled_L = 0; // TODO: evtl float
volatile int distance_traveled_L_PID = 0;
volatile int distance_traveled_R = 0; // TODO: evtl float
volatile int distance_traveled_R_PID = 0;
volatile int avg_distance_traveled = 0; // TODO: evtl float
int current_duty_cycle = 0;
enum state{slow, fast};
bool FORCE_ENCODERS = false; 

// placeholder for encoder values
int encoder_L = 0;
int encoder_R = 0;

// tick variables: [distance / (2,2cm * pi)] * 5 * 2048 Ticks
// int tick_forward = 26668 * 4 ;
// int tick_start = 6889 * 4;
// int tick_rotate = 7460 * 4;
// int tick_accelerate = 13355 * 4;

int tick_forward = 26668 * 4;
int tick_start = 6889 * 4;
int tick_rotate = 23000; // --> works for us with 90 duty rotate cycle
int tick_accelerate = 35000;


void break_immediately();
int calculate_braking_distance(int end_speed, double desired_acceleration, bool break_completely, bool both_wheels);
void accelerate_each_wheel_in_curve(int desired_speed_L, int desired_speed_R, double desired_acceleration_L, double desired_acceleration_R);
void cruise_speed_each_wheel_in_curve(int desired_speed_L, int desired_speed_R);
void accelerate(int desired_max_speed, double desired_aceeleration);
void cruise_speed(int desired_max_speed);
void decelerate(int end_speed, double desired_acceleration);
void right_turn_around();
void accelerate_each_wheel_in_rotation(int desired_speed_L, int desired_speed_R, double desired_acceleration_L, double desired_acceleration_R);
void cruise_speed_each_wheel_in_rotation(int desired_speed_L, int desired_speed_R);


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

void reset_distance_before_straight(){
    // total_angle += current_angle;
    distance_traveled_L = 0;
    distance_traveled_R = 0;
    avg_distance_traveled = 0;
}

void reset_distance_traveled_before_curve(){
    // total_angle += current_angle;
    distance_traveled_L = 0;
    distance_traveled_R = 0;
    avg_distance_traveled = 0;
}

void reset_distance_traveled_after_curve(){
    // total_angle += current_angle;
    distance_traveled_L = 0;
    distance_traveled_R = 0;
    current_angle = 0;
    avg_distance_traveled = 0;
    distance_traveled_L_PID = 0;
    distance_traveled_R_PID = 0;
}

void reset_distance_traveled_after_straight(){
    // total_angle += current_angle;
    distance_traveled_L = 0;
    distance_traveled_R = 0;
    avg_distance_traveled = 0;
}


// convert speed to duty cycle
double dutyToSpeed(int duty_cycle){
    return duty_cycle / DUTY_TO_SPEED_FACTOR;
}


int speedToDutyCycle(double speed) {
    // Map speed (0 to 800 mm/s) to duty cycle (0 to 200) duty cycle that we reach 200 with
    return static_cast<int>(speed * DUTY_TO_SPEED_FACTOR);
}


void stop(){
    ForwardLeft(0);
    ForwardRight(0);
    current_duty_cycle = 0;
    while(current_avg_speed != 0){}; // wait for the robot to really stop
}

void backup_to_wall(){
    drive_forward(365, 0, 0, 0.5); // drive forward half a cell
    right_turn_around(); // turn around 
    BackwardBoth(90); // back up to wall with duty 90
    reset_distance_traveled_after_straight();
    delay(20);
    while(current_avg_speed < 0){};
    delay(50);
    stop();
    reset_distance_traveled_after_straight();
}


void printer_debugger(int average_distance_travelled, int distance_remaining, int braking_distance){
    // cocatenate that as one string and print it to the serial monitor
    ble->print("Current Duty Cycle: " + String(current_duty_cycle) + 
           " Avg distance travelled: " + String(average_distance_travelled) + 
           " Distance remaining: " + String(distance_remaining) + 
           " Braking distance: " + String(braking_distance) + "\n");
}



// ------------------------------------------------------------------------------------
// Drive functions

void drive_forward(int desired_max_speed, int end_speed, double desired_acceleration, float squares){
    // first reset distances
    //ble->println("Driving forward");
    reset_encoder_PID_values();
    reset_distance_before_straight();
    bool reached_end_speed = false;
    bool break_completely = false;
    if (end_speed == 0){
        break_completely = true;
    }
    int desired_distance = static_cast<int>(round(squares * tick_forward)); // ticks per square * number of squares
    int ticks_until_walls_disappearing = static_cast<int>(round((squares-0.5) * tick_forward)); // start looking for walls 1 square before end
    int last_distance_traveled = 1;	// setting the distance to one so that we can enter the loop and not skip first iteration
    bool disappearing_wall_detected = false;
    int counter = 1; 
    double total_acceleration_L = 0; 
    double total_acceleration_R = 0;
    accelerate(desired_max_speed, desired_acceleration);
    while(avg_distance_traveled < desired_distance){ 
        if (last_distance_traveled == avg_distance_traveled) continue; // if the systick has not updated our values, do not update pwm values etc.
        last_distance_traveled = avg_distance_traveled; // update the last distance traveled
        int distance_remaining = desired_distance - avg_distance_traveled;
        total_acceleration_L += current_acc_L;
        total_acceleration_R += current_acc_R;
        avg_acceleration_L = total_acceleration_L / counter;
        avg_acceleration_R = total_acceleration_R / counter;
        counter++; 
        // once i reach endspeed, I set the flag to true
        if (current_avg_speed >= desired_max_speed && !reached_end_speed){
            //ble->println("Reached end speed");
            reached_end_speed = true;
        }
        int braking_distance = calculate_braking_distance(end_speed, desired_acceleration, break_completely, true);
        if (!disappearing_wall_detected &&  avg_distance_traveled > ticks_until_walls_disappearing && side_walls_disappearing()){
            desired_distance = 32400 + avg_distance_traveled; // setting desired distance to 84154 (ticks to reach next middle square)
            disappearing_wall_detected = true; // such that we only set the distance once
            //ble->println("Side walls disappearing, setting distance remaining to 32400");
        }
        if (distance_remaining > braking_distance && !reached_end_speed){
            accelerate(desired_max_speed, desired_acceleration);
            continue;
        } else if (distance_remaining > braking_distance && reached_end_speed){
            cruise_speed(desired_max_speed);
            continue;
        } else if (distance_remaining <= braking_distance && !break_completely){
            decelerate(end_speed, desired_acceleration);
            continue;
        } else if (distance_remaining <= braking_distance && break_completely){
            break_immediately();
            break;
        }
    }
    avg_acceleration_L = 0;
    avg_acceleration_R = 0;
    reset_integral_acc();
}

// calculate the duty needed for correcting
int calc_duty_correction(int desired_speed, double desired_acceleration, int correction_case){
    // calculate the error in speed
    int duty_base = speedToDutyCycle(desired_speed);
    if (current_avg_speed < 0){
        return duty_base;
    }
    int correction = 0;
    // return PID correction duty
    switch (correction_case){
        case LEFT_SPEED:
            correction = calc_correction_speed(correction_case, desired_speed, 2, 0, 0);
            break;
        case RIGHT_SPEED:
            correction = calc_correction_speed(correction_case, desired_speed, 2, 0, 0);
            break;
        case BOTH_SPEEDS:
            correction = calc_correction_speed(correction_case, desired_speed, 2, 0, 0);
            break;
        case LEFT_ACC:
            correction = calc_correction_acc(correction_case, desired_acceleration, 0.2, 0, 0); // 0.2, 0.05, 0
            break;
        case RIGHT_ACC:
            correction = calc_correction_acc(correction_case, desired_acceleration, 0.2, 0, 0); // 0.2, 0.05, 0
            break;
        case BOTH_ACC:
            correction = calc_correction_acc(correction_case, desired_acceleration, 0.2, 0, 0);
            break;
        default:
            correction = 0;
    }
    if (correction + duty_base <= 0){
        return 0;
    }
    return duty_base + correction;
}


// Function to accelerate from current speed to desired speed
void accelerate(int desired_max_speed, double desired_acceleration){
    ForwardBoth(calc_duty_correction(desired_max_speed, desired_acceleration, LEFT_ACC), calc_duty_correction(desired_max_speed, desired_acceleration, RIGHT_ACC)); // positive acceleration
}

// Function to cruise at a desired speed
void cruise_speed(int desired_max_speed){
    ForwardBoth(calc_duty_correction(desired_max_speed, 0, BOTH_SPEEDS));
}

// Function to decelerate from current speed to desired speed
void decelerate(int end_speed, double desired_acceleration){
    ForwardBoth(calc_duty_correction(end_speed, -desired_acceleration, LEFT_ACC), calc_duty_correction(end_speed, -desired_acceleration, RIGHT_ACC)); // negative acceleration
}

void break_immediately(){
    FORCE_ENCODERS = true;
    // ForwardBoth(0);
    ForwardLeft(0);
    ForwardRight(0);
    while(current_avg_speed > 0){};
    delay(10);
    FORCE_ENCODERS = false;
}


int calculate_braking_distance(int end_speed, double desired_acceleration, bool break_completely, bool both_wheels){
    // either break completely or calculate the braking distance to a desired end speed
    int current_speed = current_avg_speed;
    if (!both_wheels){
        current_speed = current_delta_speed_L;
    }
    if (!break_completely) {
        if (end_speed > current_avg_speed) {
            return -10000; // same as above
        } else if (current_avg_speed > end_speed) {
            // d = v1^2 - v2^2 / 2a [mm] -> 592.65 is the conversion factor from ticks to mm
            int braking_distance = static_cast<int>(round((current_avg_speed * current_avg_speed - end_speed * end_speed) / (2 * desired_acceleration) * 592.65));
            return braking_distance;
        } else if (end_speed == static_cast<int>(current_avg_speed)) {
            return 0;
        }
    } else {
        // Calculate the scaling factors based on the squares of the speeds
        double current_speed_square = static_cast<double>(current_avg_speed * current_avg_speed);
        double end_speed_square = static_cast<double>(end_speed * end_speed);
        double speed_slow_square = static_cast<double>(SPEED_SLOW * SPEED_SLOW);
        // Calculate the braking distance using the known distance and the scaling factors
        double braking_distance = KNOWN_BRAKE_DIST_AT_SPEED_SLOW * (current_speed_square - end_speed_square) / speed_slow_square;
        return static_cast<int>(std::round(braking_distance));
    }
    return 0;
}


// ------------------------------------------------------------------------------------
// Rotation functions



void rotate_left(){
    // start moving wheels in opposite directions (turning left)
    ForwardRight(DUTY_SLOW_ROTATION); 
    BackwardLeft(DUTY_SLOW_ROTATION);
    volatile int distance_travelled_left = 1; // setting the distance to one so that we can enter the loop and not skip first iteration
    current_duty_cycle = DUTY_SLOW_ROTATION;
    reset_distance_traveled_after_straight();
    // while we have not turned a quarter of a circle (using abs because we are travelling backwards with the left wheel)
    while(abs(distance_traveled_L) < tick_rotate && abs(distance_traveled_R) < tick_rotate) {
        // if the systick has not updated our values, do not update pwm values etc.
        if (distance_travelled_left == distance_traveled_L) continue;
        distance_travelled_left = distance_traveled_L; // update the last distance traveled (here just using the left wheel because avg distance cancels out)
        double wheel_ratio = static_cast<double>(abs(distance_traveled_L)) / static_cast<double>(abs(distance_traveled_R));
        if (wheel_ratio > 1){
            ForwardRight(DUTY_SLOW_ROTATION + static_cast<int>(wheel_ratio * 3.7)); 
            BackwardLeft(DUTY_SLOW_ROTATION);  // 
        } else {
            ForwardRight(DUTY_SLOW_ROTATION - static_cast<int>(1 / wheel_ratio * 3.7)); 
            BackwardLeft(DUTY_SLOW_ROTATION);  // 
        }
    }
    stop();
    reset_distance_traveled_after_straight();
}

void rotate_right(){
    disable_PID();
    volatile int start_angle = static_cast<int>(current_angle);
    stop();
    reset_distance_traveled_before_curve(); // wait for systick update
    BackwardRight(150); // start moving wheels in opposite directions (turning right)
    ForwardLeft(150);
    // while we have not turned a quarter of a circle (using abs because we are travelling backwards with the right wheel)
    while(abs(distance_traveled_R) < tick_rotate - 7000 || abs(distance_traveled_L) < tick_rotate - 7000){
    }
    stop();
    delay(10);
    reset_distance_traveled_after_curve();
    reset_PID_values();
    enable_PID();
    reset_integral_acc();
}

void accelerate_each_wheel_in_rotation(int desired_speed_L, int desired_speed_R, double desired_acceleration_L, double desired_acceleration_R){
    ForwardLeft(calc_duty_correction(desired_speed_L, desired_acceleration_L, LEFT_ACC));
    BackwardRight(calc_duty_correction(desired_speed_R, desired_acceleration_R, RIGHT_ACC));
}

void cruise_speed_each_wheel_in_rotation(int desired_speed_L, int desired_speed_R){
    ForwardLeft(calc_duty_correction(desired_speed_L, 0, LEFT_SPEED));
    BackwardRight(calc_duty_correction(desired_speed_R, 0, RIGHT_SPEED));
}


void right_turn_around(){
    disable_PID();
    volatile int start_angle = static_cast<int>(current_angle);
    int full_rotation_ticks = tick_rotate * 1.8;
    reset_distance_traveled_before_curve();
    delay(4); // wait for systick update
    BackwardRight(150); // start moving wheels in opposite directions (turning right)
    ForwardLeft(150);
    // while we have not turned a quarter of a circle (using abs because we are travelling backwards with the right wheel)
    while((abs(distance_traveled_R) < full_rotation_ticks || abs(distance_traveled_L) < full_rotation_ticks) && current_angle > -160 - start_angle){
    }
    stop();
    reset_distance_traveled_after_curve();
    reset_PID_values();
    enable_PID();
    delay(10);
    reset_integral_acc();
}


void turn_around_right(){
    rotate_right();
    // delay(500);
    rotate_right();
}


void turn_around_left(){
    rotate_left();
    rotate_left();
}   


// ------------------------------------------------------------------------------------
// fast-run movement curves 
double avg_acceleration_L = 0;
double avg_acceleration_R = 0;

void curve_right(){
    //ble->println("Curve right");
    disable_PID();
    double start_speed = 365;
    // if (start_speed < 365){
    //     start_speed = 365;
    // }
    // trying to fix any angle errors during curve 
    // speed calculation
    int speed_L = static_cast<int>(round((start_speed / 3) * 4));
    int speed_R = static_cast<int>((round(start_speed / 3) * 2));
    int counter = 1; 
    double total_acceleration_L = 0; 
    double total_acceleration_R = 0;
    double start_angle = current_angle;
    // total_angle += current_angle + 90;
    reset_distance_traveled_before_curve();
    current_angle = 0;
    int last_distance_traveled = avg_distance_traveled;
    accelerate_each_wheel_in_curve(speed_L, speed_R, ACCELERATION_CURVES, -ACCELERATION_CURVES);
    // less than 90 degrees because we do not want to oversteer
    while(current_angle > (-90 - start_angle)){
        if (last_distance_traveled == avg_distance_traveled) continue; // if the systick has not updated our values, do not update pwm values etc.
        last_distance_traveled = avg_distance_traveled; // update the last distance traveled
        total_acceleration_L += current_acc_L;
        total_acceleration_R += current_acc_R;
        avg_acceleration_L = total_acceleration_L / counter;
        avg_acceleration_R = total_acceleration_R / counter;
        counter++; 
        if (current_angle >= -4.68 - start_angle){
            // start to accelerate the wheels in opposite directions in curve entry
            accelerate_each_wheel_in_curve(speed_L, speed_R, ACCELERATION_CURVES, -ACCELERATION_CURVES);
            continue;
        } else if(current_angle <= - 90 + 4.68 - start_angle){
            // start to accelerate the wheels to the same speed in curve exit
            accelerate_each_wheel_in_curve(start_speed, start_speed, -ACCELERATION_CURVES, ACCELERATION_CURVES);
            continue;
        } else {
            // else we are in the curve and we want to keep the wheels at different speeds
            cruise_speed_each_wheel_in_curve(speed_L, speed_R);
            total_acceleration_L = 0; // setting values to zeri such that ehwn we slow down again that we do not have any residual acceleration
            total_acceleration_R = 0;
        }

    }
   
    // wait for the inner wheel to drive the full distance that we do not start the drive forward function too early
    // reset_distance_traveled_after_curve();
    reset_PID_values();
    enable_PID();
    reset_integral_acc();
    while(distance_traveled_R <= 48000){
        if (last_distance_traveled == avg_distance_traveled) continue; // if the systick has not updated our values, do not update pwm values etc.
        last_distance_traveled = avg_distance_traveled; // update the last distance traveled
        // ble->println("Waiting for inner wheel");
        cruise_speed_each_wheel_in_curve(start_speed, start_speed);
    }
    reset_integral_acc();
    reset_distance_traveled_after_curve();
    avg_acceleration_L = 0;
    avg_acceleration_R = 0;
}

void curve_left(){
    //ble->println("Curve left");
    disable_PID();
    double start_speed = 365;
    double start_angle = current_angle;
    int speed_L = static_cast<int>(round((start_speed / 3) * 2));
    int speed_R = static_cast<int>((round(start_speed / 3) * 4));
    int counter = 1; 
    double total_acceleration_L = 0; 
    double total_acceleration_R = 0;

    reset_distance_traveled_before_curve();
    current_angle = 0;
    int last_distance_traveled = avg_distance_traveled;
    accelerate_each_wheel_in_curve(speed_L, speed_R, -ACCELERATION_CURVES, ACCELERATION_CURVES);
    // less than 90 degrees because we do not want to oversteer
    while(current_angle < (90 - start_angle)){
        if (last_distance_traveled == avg_distance_traveled) continue; // if the systick has not updated our values, do not update pwm values etc.
        last_distance_traveled = avg_distance_traveled; // update the last distance traveled
        total_acceleration_L += current_acc_L;
        total_acceleration_R += current_acc_R;
        avg_acceleration_L = total_acceleration_L / counter;
        avg_acceleration_R = total_acceleration_R / counter;
        counter++; 
        if (current_angle <= 4.68 - start_angle){
            // start to accelerate the wheels in opposite directions in curve entry
            accelerate_each_wheel_in_curve(speed_L, speed_R, -ACCELERATION_CURVES, ACCELERATION_CURVES);
            continue;
        } else if(current_angle >= 90 - 4.68 - start_angle){
            // start to accelerate the wheels to the same speed in curve exit
            accelerate_each_wheel_in_curve(start_speed, start_speed, ACCELERATION_CURVES, -ACCELERATION_CURVES);
            continue;
        } else {
            // else we are in the curve and we want to keep the wheels at different speeds
            cruise_speed_each_wheel_in_curve(speed_L, speed_R);
            total_acceleration_L = 0; // setting values to zeri such that ehwn we slow down again that we do not have any residual acceleration
            total_acceleration_R = 0;
        }
    }
    //reset_distance_traveled_after_curve();
    reset_PID_values();
    enable_PID();
    reset_integral_acc();
    while(distance_traveled_L <= 48000){
        if (last_distance_traveled == avg_distance_traveled) continue; // if the systick has not updated our values, do not update pwm values etc.
        last_distance_traveled = avg_distance_traveled; // update the last distance traveled
        // ble->println("Waiting for inner wheel");
        cruise_speed_each_wheel_in_curve(start_speed, start_speed);
    }
    reset_distance_traveled_after_curve();
    reset_integral_acc();
    avg_acceleration_L = 0;
    avg_acceleration_R = 0;
}


// accelerate each wheel to the desired speed
void accelerate_each_wheel_in_curve(int desired_speed_L, int desired_speed_R, double desired_acceleration_L, double desired_acceleration_R){
    ForwardLeft(calc_duty_correction(desired_speed_L, desired_acceleration_L, LEFT_ACC));
    ForwardRight(calc_duty_correction(desired_speed_R, desired_acceleration_R, RIGHT_ACC));
}

void cruise_speed_each_wheel_in_curve(int desired_speed_L, int desired_speed_R){
    ForwardLeft(calc_duty_correction(desired_speed_L, 0, LEFT_SPEED));
    ForwardRight(calc_duty_correction(desired_speed_R, 0, RIGHT_SPEED));
}

// ----------------------------------------------------------------------------------------------------
// Function to drive to front wall


void drive_to_wall(){
    delay(500); // wait for wheels to really stop
    CURRENT_CASE_PID = 4;
    pid_move_function(50);
    delay(500);
}


