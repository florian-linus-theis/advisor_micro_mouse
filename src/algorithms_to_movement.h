#pragma once
#include "Setup/Setup.h"

constexpr int FORWARD = 0;
constexpr int RIGHT = 1;        // turns right, then forward - confusing
constexpr int LEFT = 3;         // turns left, then forward - confusing
constexpr int ACCELERATE = 4;
constexpr int DECELERATE = 5;
constexpr int STOP = 6;


// function to translate the actions of the algorithm into movement
std::vector<std::tuple<int, float>> translate_actions_into_movement(std::vector<int> actions, bool ballgreifer) {
    std::vector<std::tuple<int, float>> movement;
    int i = 0;
    if (ballgreifer) i = 2; // skip the first two actions

    for (; i < actions.size(); i++) {
        
        // CASE FORWARD
        if (actions[i] == FORWARD) {
            int j = 1;
            // check how many forward actions are following
            while (i + j < actions.size() && actions[i + j] == FORWARD) {
                j++;
            }
            // if there are more than one forward actions, we drive multiple squares forward
            if (j > 1) {
                movement.push_back(std::make_tuple(FORWARD, j - 1)); // j-1 because we want to decelerate after the last square
                i += j - 1;
            }
        // CASE RIGHT CURVE
        } else if (actions[i] == RIGHT) {
            if (actions[i - 1] == FORWARD) {
                movement.push_back(std::make_tuple(DECELERATE, 0.5));
            }
            movement.push_back(std::make_tuple(RIGHT, 0));
            if (actions[i + 1] == FORWARD) {
                movement.push_back(std::make_tuple(ACCELERATE, 0.5));
            }
        // CASE LEFT CURVE
        } else if (actions[i] == LEFT) {
            if (actions[i - 1] == FORWARD) {
                movement.push_back(std::make_tuple(DECELERATE, 0.5));
            }
            movement.push_back(std::make_tuple(LEFT, 0));
            if (actions[i + 1] == FORWARD) {
                movement.push_back(std::make_tuple(ACCELERATE, 0.5));
            }
        }
        // Before the last square, we decelerate to stop
        if (i == actions.size() - 1) {
            movement.push_back(std::make_tuple(DECELERATE, 0)); // entschleunigt über halbe Zelle -> bleibt auf Zellübergang stehen
            movement.push_back(std::make_tuple(STOP, 0));
        }
    }
    return movement;
}


// function to execute the translated actions of the algorithm into actual movement
// calls middle_layer_functions that work together with the hardware
void execute_movements(const std::vector<std::tuple<int, float>>& movement) {
    for (int i = 0; i < movement.size(); ++i) {
        switch (std::get<0>(movement[i])) {
            case FORWARD:
                move_forward_different(DUTY_FAST, DUTY_FAST, std::get<1>(movement[i]));
                break;
            case RIGHT:
                right_curve(DUTY_FAST_CURVE);
                break;
            case LEFT:
                left_curve(DUTY_FAST_CURVE);
                break;
            case ACCELERATE:
                move_forward_different(DUTY_FAST, DUTY_FAST, std::get<1>(movement[i]));
                break;
            case DECELERATE:
                move_forward_different(DUTY_FAST, DUTY_FAST_CURVE, std::get<1>(movement[i]));
                break;
            case STOP:
                stop(); 
                break;
            default:
                break;
        }
    }
}


