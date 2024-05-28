#pragma once
#include "API.h" // for interacting with the MMS
#include "mms_interaction.h"
#include "location.h" // importing location class
#include "maze.h" // importing maze functions
#include "state.h" // importing state class
#include "dfs.h" // importing depth-first-search algorithm
#include "bfs.h" // importing breadth-first-search algorithm
#include "a_star_nodes.h" // importing a_star_node class
#include "a_star_algorithm.h" // importing AStarAlgorithm class
#include "display.h"


constexpr bool BALLGREIFER = false; // Using the Ballgreifer Version or not?
constexpr bool MAPPING_COMPLETE = false; // Control Variable to check if the maze is already mapped
std::vector<int> GOAL_POSITION = {-1,-1}; // Global variable to store the goal position

extern int current_option; // current option selected by the user

void maze_setup(){
    display_print("Setting up maze...");
    initialize_maze(); //initializing maze
    if (BALLGREIFER == true) {
        initialize_maze_with_ballgreifer();
    }
}

void dfs_mapping(){
    if (MAPPING_COMPLETE == true) {
        display_print("Maze already mapped");
        return;
    }
    // TODO: buzzer sounds for start and end of mapping
    // log("Mapping the maze..."); 
    display_print("Mapping maze...");
    dfs_map_maze(); // Mapping the maze using depth-first search 
    set_dir(0); // Reset heading to north
    return;
}

void bfs_algorithm(){
    if (MAPPING_COMPLETE == false) {
        display_print("Maze not mapped yet");
        return;
    }
    BFSAlgorithm bfs(&maze, &GOAL_POSITION, BALLGREIFER); // Initialize BFS algorithm
    int solution_position = bfs.find_bfs_shortest_path(); // Find the shortest path using BFS algorithm (also important for a* as we need to know the goal position first)
    if (current_option == 0) {
        return; // if we have triggered external reset 
    }
    bfs.execute_shortest_path(solution_position); // Execute the shortest path found by the BFS algorithm
    return;
}

void a_star_algorithm(){
    if (MAPPING_COMPLETE == false) {
        display_print("Maze not mapped yet");
        return;
    }
    if (GOAL_POSITION[0] == -1 && GOAL_POSITION[1] == -1) {
        display_print("Goal pos not found yet");
        BFSAlgorithm bfs(&maze, &GOAL_POSITION, BALLGREIFER); // Initialize BFS algorithm
        bfs.find_bfs_shortest_path(); 
        return;
    }
    AStarAlgorithm a_star_algorithm(&maze, GOAL_POSITION, BALLGREIFER); // Initialize A* algorithm
    A_star_node* solution_a_star = a_star_algorithm.prioritize_straight_paths(); // Find the shortest path using A* algorithm
    if (current_option == 0) {
        return; // if we have triggered external reset 
    }
    a_star_algorithm.execute_shortest_path_psp(solution_a_star); // Execute the shortest path found by the A* algorithm
    return;
}
