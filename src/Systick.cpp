#include "Setup\Setup.h"


void Systick_Interrupt() {

  //Alle Steuerungsfunktionen, Messungen und sonstige Regelsystem-Funktionen werden hier untergebracht
  Distanz_Messung_Sensoren();
  delay(1000);
  printDistanzSensoren();
  //readEncoderValues();
}


void update() {

  //Motor, Sensoren und sonstiges wird hier geupdated

}