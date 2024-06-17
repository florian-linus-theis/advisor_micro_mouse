#pragma once
#include <iostream>
#include <vector>
#include "location.h"
#include "ballgrabber.h"
#include "movement.h"
#include "Setup/Setup.h"


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
std::array<bool, 4> get_walls() {
    std::array<bool, 4> walls_absolute{false, false, false, false}; // initializing list containing 4 walls each to be false by default
    std::array<bool, 4> walls_relative_cur_dir = find_walls_forward_looking(); // initializing list containing 4 walls each to be false by default
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

void move_forward_mapping_fast(int squares = 1){
    if (cur_position == std::vector<int>{0, 0}) {
        if (cur_direction == 0){
            drive_forward(SPEED_MAPPING, SPEED_MAPPING, ACCELERATION_NORMAL, 0.7778); // starter cell
            update_position();
            return;
        }
    }
    drive_forward(SPEED_MAPPING, SPEED_MAPPING, ACCELERATION_NORMAL, squares); // drive forward until edge of the cell
    update_position();
}

void turn_right(){
    rotate_right();
    update_direction(+1);

}

void fast_turn_right(){
    curve_right();
    update_direction(1);
    update_position();
}


void turn_left(){
    rotate_left();
    update_direction(-1);
}

void fast_turn_left(){
    curve_left();
    update_direction(-1);
    update_position();
}

void turn_around(){
    turn_around_right();
    update_direction(+2);
}



void turn_around_and_back_up(){
    backup_to_wall();
    update_direction(+2);
    update_position();
    drive_forward(SPEED_MAPPING, SPEED_MAPPING, ACCELERATION_NORMAL, 0.7778); // drive forward to the edge of the cell
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

void set_dir_fast_mapping(int _dir) {
    if (_dir == cur_direction) {  // If already facing the correct direction
        move_forward_mapping_fast();
        return;
    }
    if (_dir == (cur_direction + 1) % 4) {  // If need to turn right once
        fast_turn_right();
        return;
    }
    if (_dir == (cur_direction + 2) % 4) {  // If need to turn around
        if (maze[cur_position[0]][cur_position[1]].walls[cur_direction]) {
            turn_around_and_back_up();
            return;
        } else {
            drive_forward(SPEED_MAPPING, 0, ACCELERATION_NORMAL, 0.5);
            right_turn_around();
            drive_forward(SPEED_MAPPING, SPEED_MAPPING, ACCELERATION_NORMAL, 0.5);
            update_direction(+2);
            update_position();
            return;
        }
    }
    fast_turn_left();  // If need to turn left once
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

void turn_toward_fast_mapping(Location loc){
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
    set_dir_fast_mapping(_dir); // turning towards desired location
}



// ------------------------------------------------------------------------
// Algorithm Fast Run Execution
constexpr int FORWARD = 0;
constexpr int RIGHT = 1;        // turns right, then forward - confusing
constexpr int LEFT = 3;         // turns left, then forward - confusing


// function to translate the actions of the algorithm into movement
std::vector<std::tuple<int, float>> translate_actions_into_movement(std::vector<int> actions, bool ballgreifer) {
    std::vector<std::tuple<int, float>> movement;
    int i = 1; // always skipping first square (we know it is 0 action and need specific distance to drive to the edge of the cell)
    if (ballgreifer) i = 3; // skip the first three actions

    for (i; i < actions.size(); i++) {
        
        // CASE FORWARD
        if (actions[i] == FORWARD) {
            int j = 1;
            // check how many forward actions are following
            while (i + j < actions.size() && actions[i + j] == FORWARD) {
                j++;
            }
            // if there is more than one forward action, we drive multiple squares forward
            if (j > 1) {
                movement.push_back(std::make_tuple(FORWARD, j));
                i += j-1;
                continue;
            }
            // if there is only one forward action, we drive one square forward
            movement.push_back(std::make_tuple(FORWARD, 1));
        // CASE RIGHT CURVE
        } else if (actions[i] == RIGHT) {
            movement.push_back(std::make_tuple(RIGHT, 0));
        // CASE LEFT CURVE
        } else if (actions[i] == LEFT) {
            movement.push_back(std::make_tuple(LEFT, 0));
        }
    }
    return movement;
}


// function to execute the translated actions of the algorithm into actual movement
// calls middle_layer_functions that work together with the hardware
void execute_movements(const std::vector<std::tuple<int, float>>& movement) {
    if (BALLGREIFER){
        grab_ball();
    } else {
        drive_forward(SPEED_MAPPING, SPEED_MAPPING, ACCELERATION_NORMAL, 0.7778); // start square
    }
    for (int i = 0; i < movement.size(); ++i) {
        switch (std::get<0>(movement[i])) {
            case FORWARD:
                drive_forward(SPEED_MAPPING, SPEED_MAPPING, ACCELERATION_NORMAL, std::get<1>(movement[i]));
                break;
            case RIGHT:
                curve_right();
                break;
            case LEFT:
                curve_left();
                break;
            default:
                break;
        }
    }
    drive_forward(SPEED_MAPPING, 0, ACCELERATION_NORMAL, 0.5); // stop inside final square
}




