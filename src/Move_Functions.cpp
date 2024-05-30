#include "Timer_Setup.cpp"
#include "Pin_Init"

void Forward(int dutyCycle) {
  digitalToggle(PC13);
  timer3.setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
  timer3.setCaptureCompare(1, dutyCycle, PERCENT_COMPARE_FORMAT);
  timer4.setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
  timer4.setCaptureCompare(2, dutyCycle, PERCENT_COMPARE_FORMAT);
}