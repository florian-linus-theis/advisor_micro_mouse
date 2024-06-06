#pragma once
#include "Setup\Setup.h"
#include "movement.h"
#include "Music.h"

// issue 1: song needs to replace delays with interrupts
// issue 2: run ballgrabber on Timer_10 instead of Timer_4

void celebrate() {
    int counter = 0;
    Running_in_the_90s();
    ForwardLeft(50);
    BackwardRight(50);

    while (counter < 10) {
        digitalWrite(LED_RED, LOW); // on
        delay(1000);

        move_ballgrabber_forward();
        move_ballgrabber_backward();
        handle_ballgrabber();

        delay(1000);
        digitalWrite(LED_RED, HIGH); // off
        counter++;
    }

    ForwardLeft(0);
    BackwardRight(0);
}