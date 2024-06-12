#pragma once
#include "Setup/Setup.h"

constexpr int FORWARD = 0;
constexpr int RIGHT = 1;        // turns right, then forward - confusing
constexpr int LEFT = 3;         // turns left, then forward - confusing


// function to translate the actions of the algorithm into movement
std::vector<std::tuple<int, float>> translate_actions_into_movement(std::vector<int> actions, bool ballgreifer) {
    std::vector<std::tuple<int, float>> movement;
    int i = 0;
    if (ballgreifer) i = 3; // skip the first three actions

    for (i; i < actions.size(); i++) {
        
        // CASE FORWARD
        if (actions[i] == FORWARD) {
            int j = 1;
            // check how many forward actions are following
            while (i + j < actions.size() && actions[i + j] == FORWARD) {
                j++;
            }
            // if there is more than one forward action, we drive multiple squares forward
            if (j > 1) {
                movement.push_back(std::make_tuple(FORWARD, j));
                i += j-1;
                continue;
            }
            // if there is only one forward action, we drive one square forward
            movement.push_back(std::make_tuple(FORWARD, 1));
        // CASE RIGHT CURVE
        } else if (actions[i] == RIGHT) {
            movement.push_back(std::make_tuple(RIGHT, 0));
        // CASE LEFT CURVE
        } else if (actions[i] == LEFT) {
            movement.push_back(std::make_tuple(LEFT, 0));
        }
    }
    return movement;
}


// function to execute the translated actions of the algorithm into actual movement
// calls middle_layer_functions that work together with the hardware
void execute_movements(const std::vector<std::tuple<int, float>>& movement) {
    drive_forward(SPEED_FAST, 0, 0.7778); // start square
    // i = 1 because we already drove the first square
    for (int i = 1; i < movement.size(); ++i) {
        switch (std::get<0>(movement[i])) {
            case FORWARD:
                drive_forward(SPEED_MAPPING, SPEED_MAPPING, std::get<1>(movement[i]));
                break;
            case RIGHT:
                curve_right();
                break;
            case LEFT:
                curve_left();
                break;
            default:
                break;
        }
    }
    drive_forward(SPEED_MAPPING, 0, 0.5); // stop inside final square
}



