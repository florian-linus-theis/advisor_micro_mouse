#pragma once
#include "Setup/Setup.h"


enum Frequencies {
    b7 = 3729,
    a7 = 3520,
    as7 = 3322,
    g7 = 3135,
    ges7 = 2959,
    f7 = 2793,
    e7 = 2637,
    dis7 = 2489,
    d7 = 2349,
    cis7 = 2217,
    c7 = 2093,
    h6 = 1975,
    b6 = 1864,
    a6 = 1760,
    as6 = 1661,
    g6 = 1567,
    ges6 = 1479,
    f6 = 1396,
    e6 = 1318,
    es6 = 1244,
    d6 = 1174,
    des6 = 1108,
    c6 = 1046,
    h5 = 987,
    b5 = 932,
    a5 = 880,
    as5 = 830,
    g5 = 783,
    ges5 = 739,
    f5 = 698,
    e5 = 659,
    es5 = 622,
    c5 = 523,
    h4 = 493,
    as4 = 415,
    pa = 0
};

//length
double H = 750/1.5;     //halbe
double PV = 562/1.5;    //punktierte viertel
double V = 375/1.5;     //viertel 
double PA = 281/1.5;    //punktierte achtel
double A = 187/1.5;     //achtel
double S = 93/1.5;      //sechzehntel

double VT = 250/1.5;    //Viertel triole
double SQ = 75/1.5;     //Sechzehntel Quintole
double ST = 63/1.5;     //Sechzehntel Triole


//pause
double U = 360/1.5;
double Z = 270/1.5;
double L = 180/1.5;
double X = 135/1.5;
double M = 90/1.5;
double K = 45/1.5;

double VTP = 60/1.5;
double SQP = 36/1.5;
double STP = 30/1.5;


//Running in the 90s------------------------------------------------

void beat0 (){
int beat0 []      {e7, dis7, a5, e7 ,dis7 ,a5};
double length0 [] {A, A,   V,  A, A,   V};
double pause0 []  {M, M,   L,  M, M,   L};

for(int i = 0; i < sizeof(beat0) / sizeof(beat0[0]); i++){
        Buzzer_beep(beat0[i], 1, length0[i]);
        delay(pause0[i]);
        }
}

void beat1(){
int beat1 []      {e7, dis7, d7, cis7, c7, cis7, d7, dis7};
double length1 [] {A, A,   A, A,   A, A,   A, A};
double pause1 []  {M, M,   M, M,   M, M,   M, M};

for(int i = 0; i < sizeof(beat1) / sizeof(beat1[0]); i++){
        Buzzer_beep(beat1[i], 1, length1[i]);
        delay(pause1[i]);
        }
}

void beat2(){
int beat2 []      {a6, a6, e7, d7, c7, g6, a6, a6, e7, d7, c7, g6};
double length2 [] {A, S, S, S, S, A, A, S, S, S, S, A};
double pause2 []  {M, K, K, K, K ,M, M, K, K, K, K, M};

for(int i = 0; i < sizeof(beat2) / sizeof(beat2[0]); i++){
        Buzzer_beep(beat2[i], 1, length2[i]);
        delay(pause2[i]);
        }
}

void beat3(){
int beat3 []      {c7, h6, g6, c7, h6, g6, c7, h6, g6, d7, h6, g6};
double length3 [] {S, S, S, A, S, A, S, S, S, A, S, A};
double pause3 []  {K, K, K, M, K, M, K, K, K, M, K, M};

for(int i = 0; i < sizeof(beat3) / sizeof(beat3[0]); i++){
        Buzzer_beep(beat3[i], 1, length3[i]);
        delay(pause3[i]);
        }
}

void beat4(){
int beat4 []      {c7, c7, h6};
double length4 [] {H, V, V};
double pause4 []  {U, L, L};

for(int i = 0; i < sizeof(beat4) / sizeof(beat4[0]); i++){
        Buzzer_beep(beat4[i], 1, length4[i]);
        delay(pause4[i]);
        }
}

void beat5(){
int beat5 []      {a6, e6, a6, e7, d7, c7, g6, a6, e6, a6, e7, d7, c7, g6};
double length5 [] {S, S,  S, S, S, S, A, S, S,  S, S, S, S, A};
double pause5 []  {K, K,  K, K, K, K, M, K, K,  K, K, K, K, M};

for(int i = 0; i < sizeof(beat5) / sizeof(beat5[0]); i++){
        Buzzer_beep(beat5[i], 1, length5[i]);
        delay(pause5[i]);
        }
}

void beat6(){
int beat6 []      {c7, c7, c7, d7, d7, e7, e7, e7, d7, d7};
double length6 [] {S, A, S, A, A, S, A, S, A, A};
double pause6 []  {K, M, K, M, M, K, M, K, M, M};

for(int i = 0; i < sizeof(beat6) / sizeof(beat6[0]); i++){
        Buzzer_beep(beat6[i], 1, length6[i]);
        delay(pause6[i]);
        }
}

void beat7(){
int beat7 []      {a6, a6, g6, a6, a6, a6, h6, c7, d7};
double length7 [] {V, V, A, A, V, V, V, V, V};
double pause7 []  {L, L, M, M, L, L, L, L, L};

for(int i = 0; i < sizeof(beat7) / sizeof(beat7[0]); i++){
        Buzzer_beep(beat7[i], 1, length7[i]);
        delay(pause7[i]);
        }
}

void beat8(){
int beat8 []      {e7, e7, d7, d7, d7, c7};
double length8 [] {V, V, A, A, V, V, V, V, V};
double pause8 []  {L, L, M, M, L, L, L, L, L};

for(int i = 0; i < sizeof(beat8) / sizeof(beat8[0]); i++){
        Buzzer_beep(beat8[i], 1, length8[i]);
        delay(pause8[i]);
        }
}

void beat9(){
int beat9 []      {e7, e7, d7, d7, d7, c7};
double length9 [] {V, V, V, A, V, V};
double pause9 []  {L, L, L, M, L, L};

for(int i = 0; i < sizeof(beat9) / sizeof(beat9[0]); i++){
        Buzzer_beep(beat9[i], 1, length9[i]);
        delay(pause9[i]);
        }
}

void beat10(){
int beat10 []      {a6, c7, d7, d7, c7, d7, g7, e7};
double length10 [] {A, A, V, A, A, V, A, PV};
double pause10 []  {M, M, L, M, M, L, M, Z};

for(int i = 0; i < sizeof(beat10) / sizeof(beat10[0]); i++){
        Buzzer_beep(beat10[i], 1, length10[i]);
        delay(pause10[i]);
        }
}

void beat11(){
int beat11 []      {g7, e7};
double length11 [] {V,  V};
double pause11 []  {L,  L};

for(int i = 0; i < sizeof(beat11) / sizeof(beat11[0]); i++){
        Buzzer_beep(beat11[i], 1, length11[i]);
        delay(pause11[i]);
        }
}

void beat12(){
int beat12 []      {d7, c7, d7, c7, d7, c7, e7};
double length12 [] {V, A, A, A, V, A, PV};
double pause12 []  {L, M, M, M, L, M, Z};

for(int i = 0; i < sizeof(beat12) / sizeof(beat12[0]); i++){
        Buzzer_beep(beat12[i], 1, length12[i]);
        delay(pause12[i]);
        }
}

void beat13(){
int beat13 []      {d7, c7, d7, c7, d7};
double length13 [] {V, A, A, A, V};
double pause13 []  {L, M, M, M, L};

for(int i = 0; i < sizeof(beat13) / sizeof(beat13[0]); i++){
        Buzzer_beep(beat13[i], 1, length13[i]);
        delay(pause13[i]);
        }
}

void beat14(){
int beat14 []      {e7, d7, e7, d7, e7};
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


//GAS GAS GAS--------------------------------------------------------------------
void G_beat0(){
        int G_beat0 []      {es6, b5, ges6, es6, des6, pa, es6 ,c6, as4, c6, as5, as4, c6, des6, as5, h5};
        double G_length0 [] {S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S};
        double G_pause0 []  {K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K};

        for(int i = 0; i < sizeof(G_beat0) / sizeof(G_beat0[0]); i++){
        Buzzer_beep(G_beat0[i], 1, G_length0[i]);
        delay(G_pause0[i]);
        }
}

void G_beat1(){
        int G_beat1 []      {es5, ges5, h5, es5, ges5, h5, es5, ges5, des6, f5, as5, des6, ges5, b5, ges6, f6};
        double G_length1 [] {S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S};
        double G_pause1 []  {K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K};

         for(int i = 0; i < sizeof(G_beat1) / sizeof(G_beat1[0]); i++){
        Buzzer_beep(G_beat1[i], 1, G_length1[i]);
        delay(G_pause1[i]);
        }
}
void G_beat2(){
        int G_beat2 []      {es5, ges5, h5, es5, ges5, h5, es5, ges5, des6, f5, as5, des6, pa, as5, b5, des6};
        double G_length2 [] {S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S};
        double G_pause2 []  {K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K};

         for(int i = 0; i < sizeof(G_beat2) / sizeof(G_beat2[0]); i++){
        Buzzer_beep(G_beat2[i], 1, G_length2[i]);
        delay(G_pause2[i]);
        }
}
void G_beat3(){
        int G_beat3 []      {es6, h5, ges6, es6, h5, pa, es6, ges6, es6, h5, c5, b5, as5, b5, des6};
        double G_length3 [] {S, S, S, S, S, S, S, S, S, S, A, S, S, S, S};
        double G_pause3 []  {K, K, K, K, K, K, K, K, K, K, M, K, K, K, K};

         for(int i = 0; i < sizeof(G_beat3) / sizeof(G_beat3[0]); i++){
        Buzzer_beep(G_beat3[i], 1, G_length3[i]);
        delay(G_pause3[i]);
        }
}
void G_beat4(){
        int G_beat4 []      {es6, dis7, es6, d6, d7, d6};
        double G_length4 [] {PA, PA, A, PA, PA, A};
        double G_pause4 []  {X, X, M, X, X, M};

         for(int i = 0; i < sizeof(G_beat4) / sizeof(G_beat4[0]); i++){
        Buzzer_beep(G_beat4[i], 1, G_length4[i]);
        delay(G_pause4[i]);
        }
}
void G_beat5(){
        int G_beat5 []      {ges6, ges6, ges6, es6, es6, b5, des6, es6, ges6, es6, es6, es6, es6, ges6, es6, b6, as6};
        double G_length5 [] {V, V, A, A, A, A, A, A, A, H, V, A, A, V, V, H, H};
        double G_pause5 []  {L, L, M, M, M, M, M, M, M, U, L, M, M, L, L, U, U};

         for(int i = 0; i < sizeof(G_beat5) / sizeof(G_beat5[0]); i++){
        Buzzer_beep(G_beat5[i], 1, G_length5[i]);
        delay(G_pause5[i]);
        }
}
void G_beat6(){
        int G_beat6 []      {ges7, ges7, ges7, dis7, es6, b5, des6, es6, ges6, es6, as6, as6, as6, ges6, ges6, es6, es6, ges7, ges7, ges7, as7, as7, as7, pa};
        double G_length6 [] {V, V, A, A, A, A, A, A, A, H, V, A, A, A, A, A, V, A, A, V, A, A, V, V};
        double G_pause6 []  {L, L, M, M, M, M, M, M, M, U, L, M, M, M, M, M, L, M, M, L, M, M, L, L};

         for(int i = 0; i < sizeof(G_beat6) / sizeof(G_beat6[0]); i++){
        Buzzer_beep(G_beat6[i], 1, G_length6[i]);
        delay(G_pause6[i]);
        }
}
void G_beat7(){
        int G_beat7 []      {es6, es6, ges6, as6, b6, es6, es6, ges6, as6, pa, dis7, dis7, ges7, as7, pa, b7, as7, ges7, as7, b7};
        double G_length7 [] {A, A, A, V, PV, A, A, A, A, H, A, A, A, A, A, V, A, A, V, PV};
        double G_pause7 []  {M, M, M, L, Z,  M, M, M, M, U, M, M, M, M, M, L, M, M, L, Z};

         for(int i = 0; i < sizeof(G_beat7) / sizeof(G_beat7[0]); i++){
        Buzzer_beep(G_beat7[i], 1, G_length7[i]);
        delay(G_pause7[i]);
        }
}

void G_beat8(){
        int G_beat8 []      {as7, b7, as7, ges7, as7, b7, as7, ges7, pa, cis7, h6, ges6, ges6, es6, es6, es6, ges6, pa, as6, pa, b6, f6, f6, ges6, as6};
        double G_length8 [] {A, A, A, A, A, A, A, A, A, A, A, V, PV, A, A, A, A, A, A, A, V, A, A, V, PV};
        double G_pause8 []  {M, M, M, M, M, M, M, M, M, M, M, L, Z,  M, M, M, M, M, M, M, L, M, M, L, Z};

         for(int i = 0; i < sizeof(G_beat8) / sizeof(G_beat8[0]); i++){
        Buzzer_beep(G_beat8[i], 1, G_length8[i]);
        delay(G_pause8[i]);
        }
}

void G_beat9(){
        int G_beat9 []      {es6, ges6, pa, b6, es6, es6, es6, ges6, es6, des6, es6, cis7, c7, c7, h6, es6, es6, es6, ges6, as6};
        double G_length9 [] {PV, PV, S, V, A, A, A, A, A, A, PV, A, PV, A, V, A, A, A, A, PV};
        double G_pause9 []  {Z,  Z,  K, L, M, M, M, M, M, M, Z,  M, Z,  M, L, M, M, M, M, Z};

         for(int i = 0; i < sizeof(G_beat9) / sizeof(G_beat9[0]); i++){
        Buzzer_beep(G_beat9[i], 1, G_length9[i]);
        delay(G_pause9[i]);
        }
}
void G_beat10(){
        int G_beat10 []      {es6, ges6, pa, b6, es6, es6, es6, ges6, as6, ges6, b6, b6, b6, as6, ges6, as6, b6};
        double G_length10 [] {PV, PV, S, V, A, A, A, A, A, A, V, A, A, V, V, PV, H};
        double G_pause10 []  {Z,  Z,  K, L, M, M, M, M, M, M, L, M, M, L, L, Z,  U};

         for(int i = 0; i < sizeof(G_beat10) / sizeof(G_beat10[0]); i++){
        Buzzer_beep(G_beat10[i], 1, G_length10[i]);
        delay(G_pause10[i]);
        }
}
void G_beat11(){
        int G_beat11 []      {es6, b6, as6, cis7, b6, as6, cis7, b6, es6, h5, es6, h5, b6, es6, h5, es6, h5, ges6, des6};
        double G_length11 [] {V, V, A, S, A, S, S, S, A, S, S, S, S, A, S, S, S, S, A, A};
        double G_pause11 []  {L, L, M, K, M, K, K, K, M, K, K, K, K, M, K, K, K, K, M, M};

         for(int i = 0; i < sizeof(G_beat11) / sizeof(G_beat11[0]); i++){
        Buzzer_beep(G_beat11[i], 1, G_length11[i]);
        delay(G_pause11[i]);
        }
}

void G_beat12(){
        int G_beat12 []      {as6, ges6, f6, es6, as6, ges6, f6, es6, as6, ges6, f6, es6, as6, ges6, f6, es6};
        double G_length12 [] {S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S};
        double G_pause12 []  {K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K, K};

         for(int i = 0; i < sizeof(G_beat12) / sizeof(G_beat12[0]); i++){
        Buzzer_beep(G_beat12[i], 1, G_length12[i]);
        delay(G_pause12[i]);
        }
}

void G_beat13(){
        int G_beat13 []      {b6, ges6, f6, b6, ges6, f6, b6, ges6, f6, b6, ges6, f6, as7, ges7, f7, dis7, cis7, as7, ges7, f7, dis7, cis7};
        double G_length13 [] {ST, ST, ST, ST, ST, ST, ST, ST, ST, ST, ST, ST, SQ, SQ, SQ, SQ, SQ, SQ, SQ, SQ, SQ, SQ};
        double G_pause13 []  {STP, STP, STP, STP, STP, STP, STP, STP, STP, STP, STP, STP, SQP, SQP, SQP, SQP, SQP, SQP, SQP, SQP, SQP, SQP};

         for(int i = 0; i < sizeof(G_beat13) / sizeof(G_beat13[0]); i++){
        Buzzer_beep(G_beat13[i], 1, G_length13[i]);
        delay(G_pause13[i]);
        }
}

void G_beat14(){
        int G_beat14 []      {as6, des6, des6, ges6, des6, des6, f6, b5, b5, b5, b5, es6, b5, b5, ges6, f6, f6, ges5, h4, h4, ges5, h4, h4, ges5, ges6, ges6, ges6, ges7, ges7, ges7};
        double G_length14 [] {A, S, S, A, S, S, A, S, S, S, S, A, S, S, A, H, V, A, A, A, A, A, A, V, VT, VT, VT, VT, VT, VT};
        double G_pause14 []  {M, K, K, M, K, K, M, K, K, K, K, M, K, K, M, U, L, M, M, M, M, M, M, L, VTP, VTP, VTP, VTP, VTP, VTP};

         for(int i = 0; i < sizeof(G_beat14) / sizeof(G_beat14[0]); i++){
        Buzzer_beep(G_beat14[i], 1, G_length14[i]);
        delay(G_pause14[i]);
        }
}

void Gas_song(){
        G_beat0();
        G_beat1();
        G_beat0();
        G_beat2();
        G_beat0();
        G_beat1();
        G_beat3();
        G_beat4();
        G_beat5();
        G_beat6();
        G_beat7();
        G_beat8();
        G_beat9();
        G_beat10();
        G_beat9();
        G_beat10();
        G_beat11();
        G_beat12();
        G_beat13();
        G_beat0();
        G_beat1();
        G_beat0();
        G_beat2();
        G_beat0();
        G_beat1();
        G_beat3();
}

//Imperial March-----------------------------------------------------------------
void Imperial_March(){
    std::vector<int> pitches {880, 880, 880, 685, 1047, 880, 685, 1047, 880};
    std::vector<int> delayVec {500,500,500,500,250,500,250,500};
    for(int i = 0; i < pitches.size(); i++){
        Buzzer_beep(pitches[i], 1);
        delay(delayVec[i]);
    }
}


    
    




