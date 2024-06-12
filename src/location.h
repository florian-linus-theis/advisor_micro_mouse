// Using vector class in c++ for documentation see: https://en.cppreference.com/w/cpp/container/vector
#pragma once
#include <vector>
#include <iostream>
#include <FlashStorage_STM32.hpp>


class Location {
    public:
    std::array<bool, 4> walls; // list of 4 booleans (north, east, south, west)
    std::vector<int> position; // order pair list [x, y]
    bool visited; // boolean if the location has been visited
    bool ballgreifer;

    // constructor function that sets the position if a position is passed
    Location(std::vector<int> pos = {}) : position({-1, -1}), visited(false), ballgreifer(false){
        walls = {false, false, false, false};
        if (!pos.empty()) {
            position[0] = pos[0];
            position[1] = pos[1];
        }
    }
    // Serialization method
    void serialize(int& address) const {
        for (bool wall : walls) {
            EEPROM.put(address, wall);
            address += sizeof(wall);
        }
        for (int pos : position) {
            EEPROM.put(address, pos);
            address += sizeof(pos);
        }
        EEPROM.put(address, visited);
        address += sizeof(visited);
    }

    // Deserialization method
    void deserialize(int& address) {
        for (bool& wall : walls) {
            EEPROM.get(address, wall);
            address += sizeof(wall);
        }
        for (int& pos : position) {
            EEPROM.get(address, pos);
            address += sizeof(pos);
        }
        EEPROM.get(address, visited);
        address += sizeof(visited);
    }


    // takes position as order pair list [x, y]
    void set_position(std::vector<int> pos) {
        position[0] = pos[0];
        position[1] = pos[1];
    }

    // takes walls as list of 4 booleans, e.g. [true, false, true, false] and updates walls property
    void set_walls(std::array<bool, 4> walls) {
        this->walls = walls; // this-> is used to differentiate between the class attribute and the parameter that was passed
    }

    // takes boolean and updates visited property
    void set_visited(bool vis=true) {
        visited = vis;
    }

    // sets the ballgreifer property
    void set_ballgreifer(bool bg=true) {
        ballgreifer = bg;
    }

    int get_manhatten_distance(std::vector<int> goal_location) {
        return abs(position[0] - goal_location[0]) + abs(position[1] - goal_location[1]);
    }

    // takes the position of another Location Cell and return boolean whether two cells are adjacent and have no walls in between them
    bool can_move_to(Location loc) {
        return (loc.position[0] == position[0] && loc.position[1] - position[1] == +1 && !walls[0])
            || (loc.position[1] == position[1] && loc.position[0] - position[0] == +1 && !walls[1])
            || (loc.position[0] == position[0] && loc.position[1] - position[1] == -1 && !walls[2])
            || (loc.position[1] == position[1] && loc.position[0] - position[0] == -1 && !walls[3]);
    }
};
