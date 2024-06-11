#pragma once
#include "Setup/Setup.h"


//frequencies
int g7 = 3135;
int f7 = 2793;
int e = 2637;
int dis = 2489;
int d = 2349;
int cis = 2217;
int c = 2093;
int h = 1975;
int a = 1760;
int g = 1567;
int f = 1396;
int e6 = 1318;
int a5 = 880;
int vp = 0;

//length
double H = 750/1.5;     //halbe
double P = 562/1.5;     //punktierte
double V = 375/1.5;     //viertel 
double A = 187/1.5;     //achtel
double S = 93/1.5;      //sechzehntel

//pause
double U = 360/1.5;
double Z = 270/1.5;
double L = 180/1.5;
double M = 90/1.5;
double K = 45/1.5;






void Imperial_March(){
    std::vector<int> pitches {880, 880, 880, 685, 1047, 880, 685, 1047, 880};
    std::vector<int> delayVec {500,500,500,500,250,500,250,500};
    for(int i = 0; i < pitches.size(); i++){
        Buzzer_beep(pitches[i], 1);
        delay(delayVec[i]);
    }
}



//Running in the 90s---------------------------------------------------------------


//different motives
void beat0 (){
int beat0 []      {e, dis, a5, e ,dis ,a5};
double length0 [] {A, A,   V,  A, A,   V};
double pause0 []  {M, M,   L,  M, M,   L};

for(int i = 0; i < sizeof(beat0) / sizeof(beat0[0]); i++){
        Buzzer_beep(beat0[i], 1, length0[i]);
        delay(pause0[i]);
        }
}

void beat1(){
int beat1 []      {e, dis, d, cis, c, cis, d, dis};
double length1 [] {A, A,   A, A,   A, A,   A, A};
double pause1 []  {M, M,   M, M,   M, M,   M, M};

for(int i = 0; i < sizeof(beat1) / sizeof(beat1[0]); i++){
        Buzzer_beep(beat1[i], 1, length1[i]);
        delay(pause1[i]);
        }
}

void beat2(){
int beat2 []      {a, a, e, d, c, g, a, a, e, d, c, g};
double length2 [] {A, S, S, S, S, A, A, S, S, S, S, A};
double pause2 []  {M, K, K, K, K ,M, M, K, K, K, K, M};

for(int i = 0; i < sizeof(beat2) / sizeof(beat2[0]); i++){
        Buzzer_beep(beat2[i], 1, length2[i]);
        delay(pause2[i]);
        }
}

void beat3(){
int beat3 []      {c, h, g, c, h, g, c, h, g, d, h, g};
double length3 [] {S, S, S, A, S, A, S, S, S, A, S, A};
double pause3 []  {K, K, K, M, K, M, K, K, K, M, K, M};

for(int i = 0; i < sizeof(beat3) / sizeof(beat3[0]); i++){
        Buzzer_beep(beat3[i], 1, length3[i]);
        delay(pause3[i]);
        }
}

void beat4(){
int beat4 []      {c, c, h};
double length4 [] {H, V, V};
double pause4 []  {U, L, L};

for(int i = 0; i < sizeof(beat4) / sizeof(beat4[0]); i++){
        Buzzer_beep(beat4[i], 1, length4[i]);
        delay(pause4[i]);
        }
}

void beat5(){
int beat5 []      {a, e6, a, e, d, c, g, a, e6, a, e, d, c, g};
double length5 [] {S, S,  S, S, S, S, A, S, S,  S, S, S, S, A};
double pause5 []  {K, K,  K, K, K, K, M, K, K,  K, K, K, K, M};

for(int i = 0; i < sizeof(beat5) / sizeof(beat5[0]); i++){
        Buzzer_beep(beat5[i], 1, length5[i]);
        delay(pause5[i]);
        }
}

void beat6(){
int beat6 []      {c, c, c, d, d, e, e, e, d, d};
double length6 [] {S, A, S, A, A, S, A, S, A, A};
double pause6 []  {K, M, K, M, M, K, M, K, M, M};

for(int i = 0; i < sizeof(beat6) / sizeof(beat6[0]); i++){
        Buzzer_beep(beat6[i], 1, length6[i]);
        delay(pause6[i]);
        }
}

void beat7(){
int beat7 []      {a, a, g, a, a, a, h, c, d};
double length7 [] {V, V, A, A, V, V, V, V, V};
double pause7 []  {L, L, M, M, L, L, L, L, L};

for(int i = 0; i < sizeof(beat7) / sizeof(beat7[0]); i++){
        Buzzer_beep(beat7[i], 1, length7[i]);
        delay(pause7[i]);
        }
}

void beat8(){
int beat8 []      {e, e, d, d, d, c};
double length8 [] {V, V, A, A, V, V, V, V, V};
double pause8 []  {L, L, M, M, L, L, L, L, L};

for(int i = 0; i < sizeof(beat8) / sizeof(beat8[0]); i++){
        Buzzer_beep(beat8[i], 1, length8[i]);
        delay(pause8[i]);
        }
}

void beat9(){
int beat9 []      {e, e, d, d, d, c};
double length9 [] {V, V, V, A, V, V};
double pause9 []  {L, L, L, M, L, L};

for(int i = 0; i < sizeof(beat9) / sizeof(beat9[0]); i++){
        Buzzer_beep(beat9[i], 1, length9[i]);
        delay(pause9[i]);
        }
}

void beat10(){
int beat10 []      {a, c, d, d, c, d, g7, e};
double length10 [] {A, A, V, A, A, V, A, P};
double pause10 []  {M, M, L, M, M, L, M, Z};

for(int i = 0; i < sizeof(beat10) / sizeof(beat10[0]); i++){
        Buzzer_beep(beat10[i], 1, length10[i]);
        delay(pause10[i]);
        }
}

void beat11(){
int beat11 []      {g7, e};
double length11 [] {V,  V};
double pause11 []  {L,  L};

for(int i = 0; i < sizeof(beat11) / sizeof(beat11[0]); i++){
        Buzzer_beep(beat11[i], 1, length11[i]);
        delay(pause11[i]);
        }
}

void beat12(){
int beat12 []      {d, c, d, c, d, c, e};
double length12 [] {V, A, A, A, V, A, P};
double pause12 []  {L, M, M, M, L, M, Z};

for(int i = 0; i < sizeof(beat12) / sizeof(beat12[0]); i++){
        Buzzer_beep(beat12[i], 1, length12[i]);
        delay(pause12[i]);
        }
}

void beat13(){
int beat13 []      {d, c, d, c, d};
double length13 [] {V, A, A, A, V};
double pause13 []  {L, M, M, M, L};

for(int i = 0; i < sizeof(beat13) / sizeof(beat13[0]); i++){
        Buzzer_beep(beat13[i], 1, length13[i]);
        delay(pause13[i]);
        }
}

void beat14(){
int beat14 []      {e, d, e, d, e};
double length14 [] {V, A, A, A, H};
double pause14 []  {L, M, M, M, U};

for(int i = 0; i < sizeof(beat14) / sizeof(beat14[0]); i++){
        Buzzer_beep(beat14[i], 1, length14[i]);
        delay(pause14[i]);
        }
}


void Running_in_the_90s (){
    for(int w = 0; w < 2; w++){
        beat0();
        beat1();
    }
    beat2();
    beat3();
    beat2();

    beat4();

    beat2();
    beat3();
    beat2();

    beat1();

    beat5();
    beat3();
    beat5();

    beat6();
    beat5();
    beat3();
    beat5();
    beat1();

    beat7();
    beat9();
    delay(455);
    beat10();
    delay(455);
    beat11();
    beat9();

    beat12();
    beat11();
    beat9();
    beat10();

    beat11();
    beat9();
    delay(700);
    beat13();
    delay(270);
    beat14();
}





    
    




