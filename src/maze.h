#pragma once
#include "Setup/Setup.h"
#include <vector>
#include <iostream>
#include "location.h" // importing location class;
// #include "user_interface.h"

// For tracking all maze data, create a 2D vector of Locations
std::vector<std::vector<Location>> maze (MAZE_HEIGHT, std::vector<Location>(MAZE_WIDTH));
// Initialize the maze with Location objects
// maze is a global variable so no need to pass it here
void initialize_maze(){
  for (int i = 0; i < MAZE_HEIGHT; ++i) {
      for (int j = 0; j < MAZE_WIDTH; ++j) {
          maze[i][j] = Location({i, j});
          maze[i][j].set_walls({true, true, true, true}); // if we have never reached cell we set the walls so that we do not go there by accident in bfs or a-star
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

//Flash memory-------------------------------------------------------------------------
// vector to serialize maze
using Test_Maze = std::vector<std::vector<Location>>;
int buffer_size = 256 * 7; //(4+1+2) * #Squares
std::vector<int8_t> buffer(buffer_size, 0);
int indexprint;
//--------------------------------------------------------------------------------------

//check if adress in flash is available
bool is_flash_writable(uint32_t address) {
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

//transform maze into buffer vector

void serialize_maze(const std::vector<std::vector<Location>>& maze, std::vector<int8_t>& buffer) {
    buffer.clear();
    for (const auto& row : maze) {
        for (const auto& loc : row) {
            // Serialize Walls
            for(bool wall : loc.walls){
                buffer.push_back(static_cast<int8_t>(wall));
            }
            // Serialize visited
            buffer.push_back(static_cast<int8_t>(loc.visited));
            // Serialize position (assuming fixed byte order)
            buffer.push_back(static_cast<int8_t>(loc.position[0])); // Low byte
            buffer.push_back(static_cast<int8_t>(loc.position[1])); // Low byte
        }
    }
}

void write_data_to_flash(uint32_t startAddress, const std::vector<int8_t>& data) {
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);
    FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3);
    
    for (size_t i = 0; i < data.size(); i++) {
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, startAddress + i, data[i]);
    }

    HAL_FLASH_Lock();
}

void read_maze_from_flash(uint32_t startAddress, std::vector<int8_t>& buffer) {
    for (size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] = *reinterpret_cast<volatile int8_t*>(startAddress + i);
    }
}
//transform buffer vector into matrix
void deserialize_maze(const std::vector<int8_t>& buffer, std::vector<std::vector<Location>>& maze) {
    // Calculate the expected buffer size
    size_t expected_size = MAZE_HEIGHT * MAZE_WIDTH * (4 + 1 + 2); // 4 walls + visited + 2 positions
    if (buffer.size() < expected_size) {
        // Buffer size mismatch
        return;
    }

    size_t index = 0;
    for (auto& row : maze) {
        for (auto& loc : row) {
            // Deserialize walls
            for (size_t i = 0; i < loc.walls.size(); i++) {
                loc.walls[i] = static_cast<bool>(buffer[index]);
                index++;
            }
            // Deserialize visited status
            loc.visited = static_cast<bool>(buffer[index]);
            index++;
            // Deserialize position
            loc.position[0] = static_cast<int>(buffer[index]);
            loc.position[1] = static_cast<int>(buffer[index + 1]);
            index += 2;
        }
    }
}