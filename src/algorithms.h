#pragma once
 // for interacting with the MMS
#include "algo_movement_api.h"
#include "location.h" // importing location class
#include "maze.h" // importing maze functions
#include "state.h" // importing state class
#include "dfs.h" // importing depth-first-search algorithm
#include "bfs.h" // importing breadth-first-search algorithm
#include "a_star_nodes.h" // importing a_star_node class
#include "a_star_algorithm.h" // importing AStarAlgorithm class
#include "Setup/Setup.h"


bool BALLGREIFER = false; // Using the Ballgreifer Version or not?
bool MAPPING_COMPLETE = false; // Control Variable to check if the maze is already mapped
std::vector<int> POSSIBLE_GOAL_POS_ONE = {3,3}; 
std::vector<int> POSSIBLE_GOAL_POS_TWO = {8,8}; 
std::vector<int> POSSIBLE_GOAL_POS_THREE = {8,7};
std::vector<int> POSSIBLE_GOAL_POS_FOUR = {7,7}; 
std::vector<int> GOAL_POSITION = {-1,-1}; // Global variable to store the goal position


void dfs_mapping(){
    if (MAPPING_COMPLETE == true) {
        ble->println("Maze already mapped");
        return;
    } 
    cur_position = {0,0}; // Set the current position to the start position
    cur_direction = 0; // Set the current direction to north
    ble->println("Mapping maze using DFS");	
    dfs_map_maze(); // Mapping the maze using depth-first search 
    display_print("Mapping complete", 1);
    delay(1000);
    serialize_maze(maze, buffer); //Put maze into buffer vector 
    write_data_to_flash(FLASH_SECTOR_11_START_ADDR, buffer); // Save buffer vector on flash
    // Debug print to serial monitor or log
    ble->println("Serialized Buffer:");
    indexprint = 0;
    while (indexprint < buffer.size()) {
    // Print Walls
    ble->println("Walls:");
        for (int i = 0; i < 4; ++i) {
            bool wall = buffer[indexprint++] != 0;
            ble->print(wall);
            ble->print(" ");
        }
    ble->println();

    // Print Visited
    bool visited = buffer[indexprint++] != 0;
    ble->print("Visited: ");
    ble->println(visited);

    // Print Position
    ble->print("Position: [");
    int x = (buffer[indexprint]);
    int y = (buffer[indexprint + 1]); 
    ble->print(x);
    ble->print(", ");
    ble->print(y);
    ble->println("]");
    indexprint += 2; 
    }

    ble->println();

    display_print("Data written to flash", 1);
    delay(1000);
    MAPPING_COMPLETE = true; // Set mapping complete to true
    return;
}


void soft_reset(){
    // iterate through maze and set every location to unvisited
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            maze[i][j].set_visited(false);
            maze[i][j].set_walls({false, false, false, false});
        }
    }
    if (BALLGREIFER == true) {
        initialize_maze_with_ballgreifer();
    }
    MAPPING_COMPLETE = false;
}

void bfs_algorithm(){
    if (MAPPING_COMPLETE == false) {
        display->clearDisplay();
        display->print("Maze not mapped yet");
        return;
    }
    BFSAlgorithm bfs(&maze, &GOAL_POSITION, BALLGREIFER); // Initialize BFS algorithm
    int solution_position = bfs.find_bfs_shortest_path(); // Find the shortest path using BFS algorithm (also important for a* as we need to know the goal position first)
    if (current_option == 0) {
        return; // if we have triggered external reset 
    }
    // calculate the shortest path
    std::vector<int> action_vector = bfs.return_action_vector_bfs_path(solution_position);
    // print the action vector via blutooth
    ble->print("Action Vector: ");
    for (int i = 0; i < action_vector.size(); i++) {
        ble->print(action_vector[i]);
        ble->print(" ");
    }
    // translate the actions into actual movements in maze
    std::vector<std::tuple<int, float>> movements = translate_actions_into_movement(action_vector, BALLGREIFER);
    display_print("Ready for execution...", 1);
    start(5);
    // execute the movements
    execute_movements(movements);
    // display completion message
    Buzzer_beep(2000, 3); // Beep 4 times to indicate completion
    display_print("BFS complete", 1);
    delay(1000);
    return;
}

void a_star_algorithm(){
    if (MAPPING_COMPLETE == false) {
        display_print("maze not mapped yet", 1);
        Buzzer_beep(2000, 5); // Beep 4 times to indicate completion
        return;
    }
    if (GOAL_POSITION[0] == -1 && GOAL_POSITION[1] == -1) {
        delay(1000);
        display_print("Goal not found yet", 1);
        BFSAlgorithm bfs(&maze, &GOAL_POSITION, BALLGREIFER); // Initialize BFS algorithm
        bfs.find_bfs_shortest_path(); 
        delay(1000);
        display_print("Goal Position found", 1);
        delay(1000);
    }
    display_print("Calculating A*", 1);
    AStarAlgorithm a_star_algorithm(&maze, GOAL_POSITION, BALLGREIFER); // Initialize A* algorithm
    A_star_node* solution_a_star = a_star_algorithm.prioritize_straight_paths(); // Find the shortest path using A* algorithm
    if (current_option == 0) {
        return; // if we have triggered external reset 
    }
    // calculate the shortest path
    std::vector<int> action_vector = a_star_algorithm.return_action_vector_shortest_path_psp(solution_a_star);
    // translate the actions into actual movements in maze
    std::vector<std::tuple<int, float>> movements = translate_actions_into_movement(action_vector, BALLGREIFER);
    delay(500);
    display_print("Ready for execution...", 1);
    start(5);
    // execute the movements
    execute_movements(movements);
    // display completion message
    display->clearDisplay();
    Buzzer_beep(2000, 3); // Beep 4 times to indicate completion
    display->print("A* complete");
    delay(1000);
    return;
}
