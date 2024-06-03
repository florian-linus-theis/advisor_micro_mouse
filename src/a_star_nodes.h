#pragma once
#include "location.h" // importing location class; TODO: import Header file
#include <iostream>
#include <sstream> // for std::ostringstream
#include <math.h>

class A_star_node {
	public:
    Location* location; // pointer to the location object
    A_star_node* parent; // address of the parent ndoe
    // action is the action the parent took to reach this state, encoded as the 'turn' taken before moving forward
    // turn can be 0 - no turn, 1 - turn right, 2 - turn around, 3 - turn left, or -1 if null action (see default)
    int action = 0;
    int cur_dir = 0; // current direction of the mouse (0 - north, 1 - east, 2 - south, 3 - west) from where it came from
    int position_in_vec = 0; // position of the node 
    int position_parent_in_vec = 0; // position of the parent node

    int streak = 1; // cost from start to current node
    int g = 0; // cost from start to current node
    int heuristic = 0; // heuristic cost from current node to goal node
    int f = 0; // total cost of the node

    // Constructor with member initializer list
    A_star_node(Location* loc, A_star_node* parent_a_star = nullptr, int act = 0, int dir = 0,  int pos = 0, int g_cost = 0, int h_cost = 0, int f_cost = 0)
        : location(loc), parent(parent_a_star), action(act), cur_dir(dir), position_in_vec(pos), g(g_cost), heuristic(h_cost), f(f_cost) {
            set_streak();
            set_g();
        }
    
    // For comparison in the priority queue
    bool operator<(const A_star_node& other) const {
        return f > other.f; // compare the heuristic values (favors the node with the lower total cost in the priority queue)
    }

    // For printing the state
    std::string to_string() const {
        std::ostringstream oss;
        oss << "Location: (" << location->position[0] << ", " << location->position[1] << ")" << std::endl;
        oss << "Own position: " << this << std::endl; // "this" is a pointer to the current object
        oss << "Parent position: " << parent << "" << std::endl; // "this" is a pointer to the current object
        oss << "Action: " << action << std::endl;
        oss << "Current Direction: " << cur_dir << std::endl;
        oss << "G Cost: " << g << std::endl;
        oss << "H Cost: " << heuristic << std::endl;
        oss << "F Cost: " << f << std::endl;
        oss << "Streak: " << streak << std::endl;
        oss << "Position in vector: " << position_in_vec << std::endl;
        return oss.str();
    }

    // Setters
    void set_loc(Location* loc) {
        location = loc;
    }

    void set_act(int act) {
        action = act;
    }

    void set_cur_dir(int dir) {
        cur_dir = dir;
    }

    void set_heuristic(std::vector<int> goal_position) {
        heuristic = this->location->get_manhatten_distance(goal_position) - streak ; // heuristic cost from current node to goal node
    }

    void set_g() {
        if(parent == nullptr) {
            g = 0;
            return;
        }
        g = parent->g + 1; // assuming cost of moving to a new cell is 1
    }

    void set_f() {
        f = g + heuristic;
    }

    void set_streak(){
        if(parent!= nullptr && parent->cur_dir == cur_dir) {
            streak = parent->streak + 1;
        } else {
            streak = 1;
        }
    }

    void set_parent_position_in_vec(int pos) {
        position_parent_in_vec = pos;
    }

    // Getter for location
    Location get_location() {
        return *location;
    }

    // Getter for action
    int get_action() {
        return action;
    }

    // Getter for current direction
    int get_cur_dir() {
        return cur_dir;
    }
    
    // Check if the state is a goal state
    // Todo: make this relative for testing purposes
    bool is_goal() {
        return (location->position == POSSIBLE_GOAL_POS_ONE ||
                location->position == POSSIBLE_GOAL_POS_TWO ||
                location->position == POSSIBLE_GOAL_POS_THREE ||
                location->position == POSSIBLE_GOAL_POS_FOUR);
    }
};
