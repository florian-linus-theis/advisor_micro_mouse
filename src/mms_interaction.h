#pragma once
#include <iostream>
#include <vector>
#include "API.h" // for the API functions (currently still MMS API but will be changed to the new functions of mouse)
#include "location.h"


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
    std::vector<bool> walls(4, false); // initializing list containing 4 walls each to be false by default

    // Check for walls in each direction independent of the current direction 
    walls[cur_direction] = API::wallFront(); // Is there a wall in front
    walls[(cur_direction + 1) % 4] = API::wallRight(); // Is there a wall to the right
    walls[(cur_direction + 2) % 4] = false; // No wall from the direction we came from also in real scenario we do not have any sensors at the back
    walls[(cur_direction + 3) % 4] = API::wallLeft(); // Is there a wall to the left

    // If it's the first square, mark the bottom wall as there
    if (cur_position == std::vector<int>{0, 0}) {
        walls[2] = true;
    }

    return walls;
}



// ------------------------------------------------------------------------
// Navigation & Movement inside Maze


// Function to take all actions to move forward and update belief state
void move_forward() {
    API::moveForward();  // Move forward in maze
    update_position();  // Update current position
}

// Function to take all actions to turn left and update belief state
void turn_left() {
    API::turnLeft();
    update_direction(-1);  // We are turning left
}

// Function to take all actions to turn right and update belief state
void turn_right() {
    API::turnRight();
    update_direction(+1);  // We are turning right
}

// Function to take all actions to turn around
void turn_around() {
    turn_right();
    turn_right();
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

void grab_ball(){
    // Drive to the ball
    move_forward(); move_forward(); turn_right(); move_forward();
    // Drive back to the start
    turn_around(); move_forward(); turn_right();
}