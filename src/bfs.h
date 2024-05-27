#pragma once
#include <vector>
#include <algorithm>

#include "location.h"
#include "state.h"
#include "API.h"

class BFSAlgorithm {
private:
    std::vector<int> act_vector;
    std::vector<State> state_vector;
    std::vector<std::vector<Location>>* maze_ptr;
    std::vector<int>* GOAL_POSITION;
    bool BALLGREIFER = false;
    int maze_height = 16;
    int maze_width = 16;

public:
    BFSAlgorithm(std::vector<std::vector<Location>>* maze, std::vector<int>* goal_pos, bool ballgreifer) : maze_ptr(maze), GOAL_POSITION(goal_pos), BALLGREIFER(ballgreifer){
        // setting maze height and width
        maze_height = maze->size();
        maze_width = (*maze)[0].size();
    }

    int find_bfs_shortest_path() {
        // Initialize all locations to unvisited
        for (int i = 0; i < (*maze_ptr).size(); ++i) {
            for (int j = 0; j < (*maze_ptr)[i].size(); ++j) {
                (*maze_ptr)[i][j].set_visited(false);
            }
        }

        // Generate initial state: parent is self, action is null
        State first_state(&(*maze_ptr)[0][0]);
        state_vector.push_back(first_state); // Push first state to our queue
        int counter = 0;

        // while we have not every state in the state_vector
        while (counter < state_vector.size()) {
            if (counter >= maze_height*maze_width) { // If we have explored every possible state and no solution was found
                break;
            }
            State* current_state = &state_vector[counter]; // Get the first node (state) we have not yet explored from the global array (FIFO)
            current_state->location->set_visited(true);  // Mark state's location as visited

            // If it is goal, return the position of it in global state vector
            if (current_state->is_goal()) {
                // log("goal state found");
                // TODO: Buzzer success sound
                *GOAL_POSITION = current_state->location->position;
                return counter;
            }

            // Provide new references to my location and position for easier reference/better readability in code below
            Location* my_loc = current_state->location;
            int pos_0 = my_loc->position[0];
            int pos_1 = my_loc->position[1];

            // checks whether there are walls and adds possible connections from current square
            // Links the next locations to the current location if there are no walls and the next location has not been visited (current location becomes parent of next location)
            if(!my_loc->walls[0] && !(*maze_ptr)[pos_0][pos_1 + 1].visited && my_loc->can_move_to((*maze_ptr)[pos_0][pos_1 + 1]) && my_loc->ballgreifer == false) {
                State north_state(&(*maze_ptr)[pos_0][pos_1 + 1], (0 - current_state->cur_dir + 4) % 4, 0, counter);
                north_state.location->set_visited(true); // Mark the location as visited
                state_vector.push_back(north_state); // Push the state to the vector
            }
            if(!my_loc->walls[1] && !(*maze_ptr)[pos_0 + 1][pos_1].visited && my_loc->can_move_to((*maze_ptr)[pos_0 + 1][pos_1]) && my_loc->ballgreifer == false) {
                State east_state(&(*maze_ptr)[pos_0 + 1][pos_1], (1 - current_state->cur_dir + 4) % 4, 1, counter);
                east_state.location->set_visited(true); 
                state_vector.push_back(east_state);
            }
            if(!my_loc->walls[2] && !(*maze_ptr)[pos_0][pos_1 - 1].visited && my_loc->can_move_to((*maze_ptr)[pos_0][pos_1 - 1]) && my_loc->ballgreifer == false) {
                State south_state(&(*maze_ptr)[pos_0][pos_1 - 1], (2 - current_state->cur_dir + 4) % 4, 2, counter);
                south_state.location->set_visited(true); 
                state_vector.push_back(south_state);
            }
            if(!my_loc->walls[3] && !(*maze_ptr)[pos_0 - 1][pos_1].visited && my_loc->can_move_to((*maze_ptr)[pos_0 - 1][pos_1]) && my_loc->ballgreifer == false) {
                State west_state(&(*maze_ptr)[pos_0 - 1][pos_1], (3 - current_state->cur_dir + 4) % 4, 3, counter);
                west_state.location->set_visited(true);
                state_vector.push_back(west_state);
            }

            counter++;
        }
        // log("No solution found");
        // Todo OLED display Text "No solution found"
        return -1; // If no solution is found
    }

    void execute_shortest_path(int solution_position) {
        if (solution_position == -1) {  // If no solution was found
            return;
        }
        State state = state_vector[solution_position]; // Get the goal state
        while (state.action != -1) {   // While I have not reached the home position
            // log(state.to_string()); // (added)
            act_vector.push_back(state.action);  // Push action to vector
            state = state_vector[state.parent];    // Traverse up to parent
        }
        int counter = act_vector.size() - 1;

        // Accounting for the fact that we might have to grab the ball first
        if (BALLGREIFER == true) {
            grab_ball();
            counter -= 2; // So that we skip the first two actions
        }

        // Execute the actions in the maze
        while (counter >= 0) {    // start at the back of actions (at origin) and execute them in the maze until we are at the first action
            int act = act_vector[counter]; // Get action from vector
            if (act == 1) {
                turn_right();
            } else if (act == 3) {
                turn_left();
            }
            move_forward();
            counter--;
        }
    }
};