#include "Setup.h"
#include "Sensors.h"


void Systick_Interrupt() {

  //Alle Steuerungsfunktionen, Messungen und sonstige Regelsystem-Funktionen werden hier untergebracht

  //Encoder Values lese;

  Distanz_Messung_Sensoren();

  //Regelungssysteme;

}