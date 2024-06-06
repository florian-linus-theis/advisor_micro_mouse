#pragma once
#include <vector>
#include <stack>

#include "maze.h"
#include "location.h"
#include "API.h"
#include "mms_interaction.h"

extern int current_option; // current state selected by the user

// Location object stack for tracking locations that may need to be explored during mapping
std::stack<Location> loc_stack;

// Direction stack for easy backtracking through maze when a dead end is found during mapping
std::stack<int> dir_stack;

//------------------------------------------------------------------------------------
// Algorithm for Exploration
// Maps maze in depth-first search using loc_stack
// Here's how it works:
// It starts by checking if the current location has been visited. If not, it marks the location as visited, sets the walls, and marks the current position in the API.
// It then checks adjacent locations and pushes unvisited ones onto the location stack (loc_stack) for later exploration.
// It enters a loop where it pops locations from the stack until it finds an unvisited one or the stack becomes empty. If the stack becomes empty, it backtracks to the initial position.
// If it finds an unvisited location, it checks if it can move to that location from the current position. 
// If yes, it turns toward the new location, saves the current direction for backtracking, and moves forward. If not, it puts the location back on the stack, backs up one square, and tries again.
// Finally, it recursively calls itself to continue exploring the maze.
// This process repeats until all reachable locations in the maze have been visited.

void dfs_map_maze() {
    Location& cur_loc = maze[cur_position[0]][cur_position[1]]; // Create a reference to the current location object for easier reference (added)
    ble->println("in dfs");
    if (!cur_loc.visited) { // If current location has not been visited
        cur_loc.set_visited(true); // Mark location as visited
        cur_loc.set_walls(get_walls()); // Set wall locations
        ble->println("Current Walls: " + String(cur_loc.walls[0]) + ", " + String(cur_loc.walls[1]) + ", " + String(cur_loc.walls[2]) + ", " + String(cur_loc.walls[3]));

        // If there is no north wall and north location is not visited, put it on loc_stack to explore later
        if (!cur_loc.walls[0] && !maze[cur_position[0]][cur_position[1] + 1].visited && !maze[cur_position[0]][cur_position[1] + 1].ballgreifer) {
            loc_stack.push(maze[cur_position[0]][cur_position[1] + 1]);
        }

        // If there is no east wall and east location is not visited, put it on loc_stack to explore later
        if (!cur_loc.walls[1] && !maze[cur_position[0] + 1][cur_position[1]].visited && !maze[cur_position[0] + 1][cur_position[1]].ballgreifer) {
            loc_stack.push(maze[cur_position[0] + 1][cur_position[1]]);
        }

        // If there is no south wall and south location is not visited, put it on loc_stack to explore later
        if (!cur_loc.walls[2] && !maze[cur_position[0]][cur_position[1] - 1].visited && !maze[cur_position[0]][cur_position[1] - 1].ballgreifer) {
            loc_stack.push(maze[cur_position[0]][cur_position[1] - 1]);
        }

        // If there is no west wall and west location is not visited, put it on loc_stack to explore later
        if (!cur_loc.walls[3] && !maze[cur_position[0] - 1][cur_position[1]].visited && !maze[cur_position[0] - 1][cur_position[1]].ballgreifer) {
            loc_stack.push(maze[cur_position[0] - 1][cur_position[1]]);
        }
    }

    // Do-while loop to get the next available position if it exists and has not been visited already
    // recursive call inside this while-loop
    Location next_loc;
    while (true) {
        if (current_option == 0) {
            return; // if we have triggered external reset
        }
        // If loc_stack is empty, backtrack to the initial position then return
        if (loc_stack.empty()) {
            // if we are not at the origin yet, we move back and try again
            if (!(cur_position == std::vector<int>{0, 0})) {
                set_dir((dir_stack.top() + 2) % 4); // Turn around and follow direction stack in reverse order
                dir_stack.pop(); // Remove top element (added)
                move_forward_mapping();
                dfs_map_maze(); // Try to move again, recursive
            }
            return;
        }
        next_loc = loc_stack.top(); // Otherwise, take locations off of the loc_stack until we get an unvisited one
        loc_stack.pop(); // removes top element
        if (!next_loc.visited) {
            break; // if the next location has not been visited so far we break out of the loop and continue
        }
    }

    // If I can move to that location from where I am, turn toward new location, save that direction, and move forward
    if (cur_loc.can_move_to(next_loc)) {
        turn_toward(next_loc);
        dir_stack.push(cur_direction); // Save current direction for backtracking on the direction stack
        move_forward_mapping();
    } else { // Put the target location back on the loc_stack, back up one square, then try again
        loc_stack.push(next_loc);
        set_dir((dir_stack.top() + 2) % 4); // Turn toward last position
        dir_stack.pop(); // Remove top element (added)
        move_forward_mapping();
    }
    dfs_map_maze(); // Try to move again
}