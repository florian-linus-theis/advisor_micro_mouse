#pragma once
#include "Setup/Setup.h"
#include <vector>
#include <iostream>
#include "location.h" // importing location class;

// For tracking all maze data, create a 2D vector of Locations
std::vector<std::vector<Location>> maze (MAZE_HEIGHT, std::vector<Location>(MAZE_WIDTH));
int baseAddress = 0;
// Initialize the maze with Location objects
// maze is a global variable so no need to pass it here
void initialize_maze(){
  for (int i = 0; i < MAZE_HEIGHT; ++i) {
      for (int j = 0; j < MAZE_WIDTH; ++j) {
          maze[i][j] = Location({i, j});
      }
  }
}

void print_maze(){
    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        for (int j = 0; j < MAZE_WIDTH; ++j) {
            std::cerr << maze[i][j].position[0] << " " << maze[i][j].position[1] << std::endl;
            std::cerr << &maze[i][j] << std::endl;
        }
    }
    
}

// To initialize all walls in the Ballgreifer-Area
void initialize_maze_with_ballgreifer(){
    // first cell
    maze[1][0].set_visited(true);
    maze[1][0].set_walls({false, false, true, true});
    maze[1][0].set_ballgreifer(true);
    // second cell
    maze[2][0].set_visited(true);
    maze[2][0].set_walls({false, true, true, false});
    maze[2][0].set_ballgreifer(true);
    // third cell
    maze[1][1].set_visited(true);
    maze[1][1].set_walls({false, false, false, false});
    maze[1][1].set_ballgreifer(true);
    // fourth cell
    maze[2][1].set_visited(true);
    maze[2][1].set_walls({false, true, false, false});
    maze[2][1].set_ballgreifer(true);
    // fifth cell
    maze[1][2].set_visited(true);
    maze[1][2].set_walls({true, false, false, false});
    maze[1][2].set_ballgreifer(true);
    // sixth cell
    maze[2][2].set_visited(true);
    maze[2][2].set_walls({true, true, false, false});
    maze[2][2].set_ballgreifer(true);
}


void maze_setup(){
    // display_print("Setting up maze...");
    initialize_maze(); //initializing maze
    if (BALLGREIFER == true) {
        initialize_maze_with_ballgreifer();
    }
}

void safeMazeToFlash(const std::vector<std::vector<Location>>& maze, int baseAddress) {
    // Store the dimensions first
    int height = maze.size();
    int width = maze[0].size();
    EEPROM.put(baseAddress, height);
    baseAddress += sizeof(height);
    EEPROM.put(baseAddress, width);
    baseAddress += sizeof(width);

    // Store each Location object
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            maze[i][j].serialize(baseAddress);
        }
    }
    EEPROM.commit();
}

void loadMazeFromFlash(std::vector<std::vector<Location>>& maze, int baseAddress) {
    int height;
    int width;

    // Read the dimensions first
    EEPROM.get(baseAddress, height);
    baseAddress += sizeof(height);
    EEPROM.get(baseAddress, width);
    baseAddress += sizeof(width);

    maze.resize(height, std::vector<Location>(width));

    // Read each Location object
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            maze[i][j].deserialize(baseAddress);
        }
    }
}

