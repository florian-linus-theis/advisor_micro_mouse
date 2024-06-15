#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include "a_star_nodes.h"
#include <iostream>
 // for the API functions (currently still MMS API but will be changed to the new functions of mouse)
#include "algo_movement_api.h"

extern int current_option; // current state selected by the user

class AStarAlgorithm {
private:
    std::vector<A_star_node> state_vector_psp;
    std::priority_queue<A_star_node> openSet_psp;
    std::vector<std::vector<Location>>* maze_ptr;
    std::vector<int> goal_position;
    int maze_height = 16;
    int maze_width = 16;
    bool BALLGREIFER = false;

public:
    // Constructor to initialize the maze pointer
    AStarAlgorithm(std::vector<std::vector<Location>>* maze, std::vector<int> goal, bool ballgreifer) : maze_ptr(maze), goal_position(goal), BALLGREIFER(ballgreifer) {
        // setting maze height and width
        maze_height = maze->size();
        maze_width = (*maze)[0].size();
    }


    // Function to prioritize straight paths in the maze
    A_star_node* prioritize_straight_paths() {
        // mark all locations as unvisited
        for (size_t i = 0; i < maze_ptr->size(); ++i) {
            for (size_t j = 0; j < (*maze_ptr)[i].size(); ++j) {
                (*maze_ptr)[i][j].set_visited(false);
            }
        }
        // Generate initial state: parent is self, action is null
        A_star_node first_state(&(*maze_ptr)[0][0]);
        first_state.set_heuristic(goal_position);
        first_state.set_f();
        first_state.set_parent_position_in_vec(-1);
        state_vector_psp.push_back(first_state); // Push first state constant vector
        openSet_psp.push(first_state); // Push first state to our priority queue

        int counter = 1; 
        // while we have not every state in the state_vector_psp
        while (true) {
            if (counter >= maze_height*maze_width) { // If we have explored every possible state and no solution was found
                break;
            }
            A_star_node* current_state = &state_vector_psp[openSet_psp.top().position_in_vec]; // Get the first node (state) we have not yet explored from the global array (FIFO)
            openSet_psp.pop();
            current_state->location->set_visited(true);  // Mark state's location as visited
            
            // If it is goal, return the position of it in global state vector
            if (current_state->is_goal()) {
                display_print("Goal state found", 1);
                return current_state;
            }

            // Provide new references to my location and position for easier reference/better readability in code below
            Location* my_loc = current_state->location;
            int pos_0 = my_loc->position[0];
            int pos_1 = my_loc->position[1];

            // checks whether there are walls and adds possible connections from current square
            // Links the next locations to the current location if there are no walls and the next location has not been visited (current location becomes parent of next location)
            if(!my_loc->walls[0] && !(*maze_ptr)[pos_0][pos_1 + 1].visited && my_loc->can_move_to((*maze_ptr)[pos_0][pos_1 + 1]) && my_loc->ballgreifer == false) {
                A_star_node north_state(&(*maze_ptr)[pos_0][pos_1 + 1], current_state, (0 - current_state->cur_dir + 4) % 4, 0, counter); // Create new state
                set_state_and_push_to_queue(&north_state); // Set state and push to queue
                counter++;
            }
            // Same logic as above for the other directions
            if(!my_loc->walls[1] && !(*maze_ptr)[pos_0 + 1][pos_1].visited && my_loc->can_move_to((*maze_ptr)[pos_0 + 1][pos_1]) && my_loc->ballgreifer == false) {
                A_star_node east_state(&(*maze_ptr)[pos_0 + 1][pos_1], current_state, (1 - current_state->cur_dir + 4) % 4, 1, counter);
                set_state_and_push_to_queue(&east_state);
                counter++;
            }
            if(!my_loc->walls[2] && !(*maze_ptr)[pos_0][pos_1 - 1].visited && my_loc->can_move_to((*maze_ptr)[pos_0][pos_1 - 1]) && my_loc->ballgreifer == false) {
                A_star_node south_state(&(*maze_ptr)[pos_0][pos_1 - 1], current_state, (2 - current_state->cur_dir + 4) % 4, 2, counter);
                set_state_and_push_to_queue(&south_state);
                counter++;
            }
            if(!my_loc->walls[3] && !(*maze_ptr)[pos_0 - 1][pos_1].visited && my_loc->can_move_to((*maze_ptr)[pos_0 - 1][pos_1]) && my_loc->ballgreifer == false) {
                A_star_node west_state(&(*maze_ptr)[pos_0 - 1][pos_1], current_state, (3 - current_state->cur_dir + 4) % 4, 3, counter);
                set_state_and_push_to_queue(&west_state);
                counter++;
            }       
        }
        return nullptr; // If no solution is found
    } 

    void set_state_and_push_to_queue(A_star_node* state){
        state->location->set_visited(true); // Mark the location as visited
        state->set_heuristic(goal_position); // Set heuristic
        state->set_f(); // Set f
        state->set_parent_position_in_vec(state->parent->position_in_vec); // Set parent position in vector
        state_vector_psp.push_back(*state); // Push state to vector
        openSet_psp.push(*state); // Push state to priority queue
    }


    // Function to execute the shortest path found by the A* algorithm
    std::vector<int> return_action_vector_shortest_path_psp(A_star_node* solution) {
        if (solution == nullptr) {  // If no solution was found
            display_print("No solution found", 1);
            return {-1}; // Return -1 to indicate no solution found
        }
        
        std::vector<int> act_vector_psp;
        A_star_node temp = *solution; 

        do {
            act_vector_psp.push_back(temp.action); // Push action to vector
            temp = state_vector_psp[temp.position_parent_in_vec];   // Traverse up to parent
        } while (temp.position_parent_in_vec != -1);

        // reverse the vector to get the correct order of actions
        reverse(act_vector_psp.begin(), act_vector_psp.end());

       return act_vector_psp;
    }
};