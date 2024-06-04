#pragma once
#include "Setup\Setup.h"

void Imperial_March(){
    std::vector<int> pitches {880, 880, 880, 685, 1047, 880, 685, 1047, 880};
            std::vector<int> delayVec {500,500,500,500,250,500,250,500};
            for(int i = 0; i < pitches.size(); i++){
                Buzzer_beep(pitches[i], 1);
                delay(delayVec[i]);
            }
}

void Running_in_the_90s(){

}