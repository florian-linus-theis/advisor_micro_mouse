#pragma once
#include "Setup/Setup.h"
#include <vector>
#include <iostream>
#include "location.h" // importing location class;

// For tracking all maze data, create a 2D vector of Locations
std::vector<std::vector<Location>> maze (MAZE_HEIGHT, std::vector<Location>(MAZE_WIDTH));
int baseAddress = 0x08080000;
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

//Flash memory---------------------------------------------------------------------

//check if adress in flash is available
bool isFlashWritable(uint32_t address) {
    const uint32_t FLASH_START_ADDRESS = 0x08000000;  // Flash memory start address
    const uint32_t FLASH_SECTOR_SIZE  = 131072;      // Flash memory sector size in bytes (128 KB)
    const uint32_t FLASH_SIZE = 1048576;            // Total Flash memory size in bytes (1 MB)

    // Check if address is within the Flash memory size
    if (address >= FLASH_START_ADDRESS && address < (FLASH_START_ADDRESS + FLASH_SIZE)) {

        // Calculate the sector number
        uint32_t sector_number = (address - FLASH_START_ADDRESS) / FLASH_SECTOR_SIZE;
        
        // Check if flash sector is fully erased
        for (uint32_t i = 0; i < FLASH_SECTOR_SIZE; i += 4) {           // STM32F4 reads and erases 32 bits words (4 bytes) 
            if (*reinterpret_cast<const volatile uint32_t*>(FLASH_START_ADDRESS + (sector_number * FLASH_SECTOR_SIZE) + i) != 0xFFFFFFFF) {
                return false;
            }
        }
        return true; // Flash sector  fully erased and writable
    }
    return false; // Address outside of Flash memory 
}

void saveMazeToFlash(const std::vector<std::vector<Location>>& maze, int baseAddress) {
    HAL_StatusTypeDef status = HAL_FLASH_Unlock();
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
             baseAddress += sizeof(Location); // Increment baseAddress
        }
    }
    EEPROM.commit();
    HAL_FLASH_Lock();
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

