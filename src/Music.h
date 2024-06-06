#pragma once
#include "Setup\Setup.h"

int g = 1567;
int a = 1760;
int h = 1975;
int c = 2093;
int d = 2349;
int e = 2637;

double H = 750/1.5;
double V = 375/1.5;
double A = 187/1.5;
double S = 93/1.5;

double K = 45/1.5;
double M = 90/1.5;
double L = 180/1.5;
double U = 360/1.5;


void Imperial_March(){
    std::vector<int> pitches {880, 880, 880, 685, 1047, 880, 685, 1047, 880};
    std::vector<int> delayVec {500,500,500,500,250,500,250,500};
    for(int i = 0; i < pitches.size(); i++){
        Buzzer_beep(pitches[i], 1);
        delay(delayVec[i]);
    }
}

void Running_in_the_90s(){
    //Viertel: 375, Achtel: 187.5, Sechzehntel: 93.75
    
           int notes[] {a, a, e, d, c, g, a, a, e, d, c, g, c, h, g, c, h, g, c, h, g, d, h, g, a, a, e, d, c, g, a, a, e, d, c, g, c, c, h};
    double delayVec1[] {A, S, S, S, S, A, A, S, S, S, S, A, S, S, S, A, S, A, S, S, S, A, S, A, A, S, S, S, S, A, A, S, S, S, S, A, H, V, V};
        double pause[] {M, K, K, K, K ,M, M, K, K, K, K, M, K, K, K, M, K, M, K, K, K, M, K, M, M, K, K, K, K, M, M, K, K, K, K, M, U, L, L};

    int size = sizeof(notes) / sizeof(notes[0]);

    for(int r=0; r<5; r++){
        for(int i = 0; i < size; i++){
            Buzzer_beep(notes[i], 1, delayVec1[i]);
            delay(pause[i]);
        }
    }
}