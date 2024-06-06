#pragma once
#include <iostream>
#include <vector>
#include "API.h" // for the API functions (currently still MMS API but will be changed to the new functions of mouse)
#include "location.h"
#include <ballgrabber.h>
#include "display.h"
#include <movement.h>
#include "Setup\Setup.h"


// For tracking current global direction
// 0 = North, 1 = East, 2 = South, 3 = West
int cur_direction = 0;

// For tracking current global 'physical' position in maze as [x, y], initialized to [0, 0] (i.e. bottom left cell, origin)
std::vector<int> cur_position = {0, 0};

// Function to update position by one cell in the maze
// Move direction can only be range 0-3 with 0 being North
void update_position() {
    if (cur_direction == 0) {    // facing north
        cur_position[1] += 1;
    } else if (cur_direction == 1) {  // facing east
        cur_position[0] += 1;
    } else if (cur_direction == 2) {  // facing south
        cur_position[1] -= 1;
    } else if (cur_direction == 3) {  // facing west
        cur_position[0] -= 1;
    }
}


// Function to update direction
// Takes a turn direction (0-3) and updates current direction based on that
void update_direction(int turn_direction) {
    cur_direction = (cur_direction + turn_direction + 4) % 4; // Adding 4 ensures that the result is always positive, preventing negative values from the modulo operation.
}


// Function to get the list of walls around the current cell in the maze, e.g. [True, False, True, False] -> No walls to the front and the back of the mouse but left and right
// returns values relative to direction we come from
std::vector<bool> get_walls() {
    std::vector<bool> walls_absolute(4, false); // initializing list containing 4 walls each to be false by default
    std::vector<bool> walls_relative_cur_dir = find_walls(); // initializing list containing 4 walls each to be false by default
    // Check for walls in each direction independent of the current direction 
    walls_absolute[cur_direction] = walls_relative_cur_dir[0]; // Is there a wall in front
    walls_absolute[(cur_direction + 1) % 4] = walls_relative_cur_dir[1]; // Is there a wall to the right
    walls_absolute[(cur_direction + 2) % 4] = false; // No wall from the direction we came from also in real scenario we do not have any sensors at the back
    walls_absolute[(cur_direction + 3) % 4] = walls_relative_cur_dir[3]; // Is there a wall to the left

    // If it's the first square, mark the bottom wall as there
    if (cur_position == std::vector<int>{0, 0}) {
        walls_absolute[2] = true;
    }

    return walls_absolute;
}



// ------------------------------------------------------------------------
// Navigation & Movement inside Maze


// Function to take all actions to move forward and update belief state
void move_forward_fast(int squares = 1) {
    if(current_option == MODE_STANDBY) return;
    int duty_cycle = DUTY_SLOW;
    if (current_option == MODE_BFS || current_option == MODE_ASTAR) {
        duty_cycle = DUTY_FAST;
    }
    move_forward_middle_level(duty_cycle, squares);  // Move forward in maze
    update_position();  // Update current position
}

void move_forward_mapping(int squares = 1){
    move_forward_different(DUTY_SLOW, 0, squares);
    recalibrate_front_wall();
    update_position();
}

// Function to take all actions to turn left and update belief state
void fast_turn_left() {
    left_curve(DUTY_FAST_CURVE);
    update_direction(-1);  // We are turning left
}

// Function to take all actions to turn right and update belief state
void fast_turn_right() {
    right_curve(DUTY_FAST_CURVE);
    update_direction(+1);  // We are turning right
}

void turn_right(){
    rotate_right();
    update_direction(+1);

}

void turn_left(){
    rotate_left();
    update_direction(-1);
}

void turn_around(){
    turn_around_right();
    update_direction(+2);
}

// Function to change current direction to a specific direction
void set_dir(int _dir) {
    if (_dir == cur_direction) {  // If already facing the correct direction
        return;
    }
    if (_dir == (cur_direction + 1) % 4) {  // If need to turn right once
        turn_right();
        return;
    }
    if (_dir == (cur_direction + 2) % 4) {  // If need to turn around
        turn_around();
        return;
    }
    turn_left();  // If need to turn left once
}

// Function to turn toward an adjacent location object
void turn_toward(Location loc) {
    int _dir = cur_direction;
    // Find direction of adjacent location
    if (cur_position[0] == loc.position[0]) {  // If two locations have the same x coordinate
        if ((cur_position[1] - loc.position[1]) == 1) {  // If mouse is "above" the next location, turn south
            _dir = 2;
        } else {  // Otherwise, mouse must be below the next location
            _dir = 0;
        }
    } else {  // Two directions have the same y coordinate
        if ((cur_position[0] - loc.position[0]) == 1) {  // If mouse is to the right of location, turn west
            _dir = 3;
        } else {  // Mouse must be to the left of the location
            _dir = 1;
        }
    }
    set_dir(_dir); // turning towards desired location
}

