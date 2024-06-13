#include "Setup\Setup.h"


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

volatile int note_cnt = 0;
volatile int beat_cnt = 0;
volatile bool SONG_COMPLETE = false;

//length
int H = 500;     //halbe
int P = 375;     //punktierte
int V = 250;     //viertel 
int A = 125;     //achtel
int S = 63;      //sechzehntel

//pause
int U = 240;
int Z = 180;
int L = 120;
int M = 60;
int K = 30;






void Imperial_March(){
    std::vector<int> pitches {880, 880, 880, 685, 1047, 880, 685, 1047, 880};
    std::vector<int> delayVec {500,500,500,500,250,500,250,500};
    for(int i = 0; i < pitches.size(); i++){
        Buzzer_beep(pitches[i], 1);
        delay(delayVec[i]);
    }
}



//Running in the 90s---------------------------------------------------------------


int beat0 []      {e, dis, a5, e ,dis ,a5};
int length0 [] {A, A,   V,  A, A,   V};
int pause0 []  {M, M,   L,  M, M,   L};


int beat1 []      {e, dis, d, cis, c, cis, d, dis};
int length1 [] {A, A,   A, A,   A, A,   A, A};
int pause1 []  {M, M,   M, M,   M, M,   M, M};

int beat2 []      {a, a, e, d, c, g, a, a, e, d, c, g};
int length2 [] {A, S, S, S, S, A, A, S, S, S, S, A};
int pause2 []  {M, K, K, K, K ,M, M, K, K, K, K, M};

int beat3 []      {c, h, g, c, h, g, c, h, g, d, h, g};
int length3 [] {S, S, S, A, S, A, S, S, S, A, S, A};
int pause3 []  {K, K, K, M, K, M, K, K, K, M, K, M};

int beat4 []      {c, c, h};
int length4 [] {H, V, V};
int pause4 []  {U, L, L};

int beat5 []      {a, e6, a, e, d, c, g, a, e6, a, e, d, c, g};
int length5 [] {S, S,  S, S, S, S, A, S, S,  S, S, S, S, A};
int pause5 []  {K, K,  K, K, K, K, M, K, K,  K, K, K, K, M};

int beat6 []      {c, c, c, d, d, e, e, e, d, d};
int length6 [] {S, A, S, A, A, S, A, S, A, A};
int pause6 []  {K, M, K, M, M, K, M, K, M, M};

int beat7 []      {a, a, g, a, a, a, h, c, d};
int length7 [] {V, V, A, A, V, V, V, V, V};
int pause7 []  {L, L, M, M, L, L, L, L, L};

int beat8 []      {e, e, d, d, d, c};
int length8 [] {V, V, A, A, V, V, V, V, V};
int pause8 []  {L, L, M, M, L, L, L, L, L};

int beat9 []      {e, e, d, d, d, c};
int length9 [] {V, V, V, A, V, V};
int pause9 []  {L, L, L, M, L, L};

int beat10 []      {a, c, d, d, c, d, g7, e};
int length10 [] {A, A, V, A, A, V, A, P};
int pause10 []  {M, M, L, M, M, L, M, Z};

int beat11 []      {g7, e};
int length11 [] {V,  V};
int pause11 []  {L,  L};

int beat12 []      {d, c, d, c, d, c, e};
int length12 [] {V, A, A, A, V, A, P};
int pause12 []  {L, M, M, M, L, M, Z};

int beat13 []      {d, c, d, c, d};
int length13 [] {V, A, A, A, V};
int pause13 []  {L, M, M, M, L};

int beat14 []      {e, d, e, d, e};
int length14 [] {V, A, A, A, H};
int pause14 []  {L, M, M, M, U};

int beat15[] {vp};
int length15[] {445};
int pause15[] {10};

int beat16[] {vp};
int length16[] {700};
int pause16[] {10};

int beat17[] {vp};
int length17[] {260};
int pause17[] {10};



//different motives
void func_beat0 (){
for(int i = 0; i < sizeof(beat0) / sizeof(beat0[0]); i++){
        Buzzer_beep(beat0[i], 1, length0[i]);
        delay(pause0[i]);
        }
}

void func_beat1(){
for(int i = 0; i < sizeof(beat1) / sizeof(beat1[0]); i++){
        Buzzer_beep(beat1[i], 1, length1[i]);
        delay(pause1[i]);
        }
}

void func_beat2(){
for(int i = 0; i < sizeof(beat2) / sizeof(beat2[0]); i++){
        Buzzer_beep(beat2[i], 1, length2[i]);
        delay(pause2[i]);
        }
}

void func_beat3(){
for(int i = 0; i < sizeof(beat3) / sizeof(beat3[0]); i++){
        Buzzer_beep(beat3[i], 1, length3[i]);
        delay(pause3[i]);
        }
}

void func_beat4(){
for(int i = 0; i < sizeof(beat4) / sizeof(beat4[0]); i++){
        Buzzer_beep(beat4[i], 1, length4[i]);
        delay(pause4[i]);
        }
}

void func_beat5(){
for(int i = 0; i < sizeof(beat5) / sizeof(beat5[0]); i++){
        Buzzer_beep(beat5[i], 1, length5[i]);
        delay(pause5[i]);
        }
}

void func_beat6(){
for(int i = 0; i < sizeof(beat6) / sizeof(beat6[0]); i++){
        Buzzer_beep(beat6[i], 1, length6[i]);
        delay(pause6[i]);
        }
}

void func_beat7(){
for(int i = 0; i < sizeof(beat7) / sizeof(beat7[0]); i++){
        Buzzer_beep(beat7[i], 1, length7[i]);
        delay(pause7[i]);
        }
}

void func_beat8(){
for(int i = 0; i < sizeof(beat8) / sizeof(beat8[0]); i++){
        Buzzer_beep(beat8[i], 1, length8[i]);
        delay(pause8[i]);
        }
}

void func_beat9(){
for(int i = 0; i < sizeof(beat9) / sizeof(beat9[0]); i++){
        Buzzer_beep(beat9[i], 1, length9[i]);
        delay(pause9[i]);
        }
}

void func_beat10(){
for(int i = 0; i < sizeof(beat10) / sizeof(beat10[0]); i++){
        Buzzer_beep(beat10[i], 1, length10[i]);
        delay(pause10[i]);
        }
}

void func_beat11(){
for(int i = 0; i < sizeof(beat11) / sizeof(beat11[0]); i++){
        Buzzer_beep(beat11[i], 1, length11[i]);
        delay(pause11[i]);
        }
}

void func_beat12(){
for(int i = 0; i < sizeof(beat12) / sizeof(beat12[0]); i++){
        Buzzer_beep(beat12[i], 1, length12[i]);
        delay(pause12[i]);
        }
}

void func_beat13(){
for(int i = 0; i < sizeof(beat13) / sizeof(beat13[0]); i++){
        Buzzer_beep(beat13[i], 1, length13[i]);
        delay(pause13[i]);
        }
}

void func_beat14(){
for(int i = 0; i < sizeof(beat14) / sizeof(beat14[0]); i++){
        Buzzer_beep(beat14[i], 1, length14[i]);
        delay(pause14[i]);
        }
}



// void Running_in_the_90s (){
//     for(int w = 0; w < 2; w++){
//         func_beat0();
//         func_beat1();
//     }
//     func_beat2();
//     func_beat3();
//     func_beat2();

//     func_beat4();

//     func_beat2();
//     func_beat3();
//     func_beat2();

//     func_beat1();

//     func_beat5();
//     func_beat3();
//     func_beat5();

//     func_beat6();
//     func_beat5();
//     func_beat3();
//     func_beat5();
//     func_beat1();

//     func_beat7();
//     func_beat9();
//     delay(455);
//     func_beat10();
//     delay(455);
//     func_beat11();
//     func_beat9();

//     func_beat12();
//     func_beat11();
//     func_beat9();
//     func_beat10();

//     func_beat11();
//     func_beat9();
//     delay(700);
//     func_beat13();
//     delay(270);
//     func_beat14();
// }


 int* full_90s_notes[] = {
        beat0, 
        beat1, 
        beat0, 
        beat1,   
        beat2,
        beat3,
        beat2,
        beat4,
        beat2,
        beat3,
        beat2,
        beat1,
        beat5,
        beat3,
        beat5,
        beat6,
        beat5,
        beat3,
        beat5,
        beat1,
        beat7,
        beat9,
        beat15,
        beat10,
        beat15,
        beat11,
        beat9,
        beat12,
        beat11,
        beat9,
        beat10,
        beat11,
        beat9,
        beat16,
        beat13,
        beat17,
        beat14, };

int* full_90s_length[] = {
        length0, 
        length1, 
        length0, 
        length1,   
        length2,
        length3,
        length2,
        length4,
        length2,
        length3,
        length2,
        length1,
        length5,
        length3,
        length5,
        length6,
        length5,
        length3,
        length5,
        length1,
        length7,
        length9,
        length15,
        length10,
        length15,
        length11,
        length9,
        length12,
        length11,
        length9,
        length10,
        length11,
        length9,
        length16,
        length13,
        length17,
        length14, };

int* full_90s_pause[] = {
        pause0, 
        pause1, 
        pause0, 
        pause1,   
        pause2,
        pause3,
        pause2,
        pause4,
        pause2,
        pause3,
        pause2,
        pause1,
        pause5,
        pause3,
        pause5,
        pause6,
        pause5,
        pause3,
        pause5,
        pause1,
        pause7,
        pause9,
        pause15,
        pause10,
        pause15,
        pause11,
        pause9,
        pause12,
        pause11,
        pause9,
        pause10,
        pause11,
        pause9,
        pause16,
        pause13,
        pause17,
        pause14, };



void Running_in_the_90s(){
    note_cnt = 0;
    beat_cnt = 0;
    SONG_COMPLETE = false;
    SETUP_COMPLETE = false;
    timer13->setCount(0);
    timer13->refresh();
    timer13->resume();
    SETUP_COMPLETE = true;

    while (!SONG_COMPLETE);
    timer13->pause();
}


void Timer13_Interrupt() {
        if(!SETUP_COMPLETE) return;

        SETUP_COMPLETE = false;
        timer13->setOverflow(10*(full_90s_pause[beat_cnt][note_cnt] + full_90s_length[beat_cnt][note_cnt]), TICK_FORMAT);
        SETUP_COMPLETE = true;

        Buzzer_beep_noBlock(full_90s_notes[beat_cnt][note_cnt], 1, full_90s_length[beat_cnt][note_cnt]);
        
        ble->println("b_cnt: " + String(beat_cnt) + "n_cnt: " + String(note_cnt) + "note: " + String(full_90s_notes[beat_cnt][note_cnt]) + " length: " + String(full_90s_length[beat_cnt][note_cnt]) + "  pause: " + String(full_90s_pause[beat_cnt][note_cnt]));

        note_cnt++;

        if (note_cnt >= sizeof(full_90s_notes[beat_cnt]) / sizeof(full_90s_notes[beat_cnt][0]) ) {
                note_cnt = 0;
                beat_cnt++;
        }
        if (beat_cnt > 36) SONG_COMPLETE = true;  
} 