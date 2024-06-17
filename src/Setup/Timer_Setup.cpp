#include "Setup/Setup.h"

/*
Clock Overview:
    Processor Core Clock: 168MHz
    APB1 Timer Clock:     84MHz     Timers: 2, 3, 4, 5, 6, 7, 12, 13, 14
    APB2 Timer Clock:     168MHz    Timers: 1, 8, 9, 10, 11
*/


//unused Timers 8,9,11,12,13

HardwareTimer *timer14 = new HardwareTimer(TIM14);
HardwareTimer *timer3 = new HardwareTimer(TIM3);
HardwareTimer *timer4 = new HardwareTimer(TIM4);
HardwareTimer *timer2 = new HardwareTimer(TIM2);
HardwareTimer *timer5 = new HardwareTimer(TIM5);
HardwareTimer *timer6 = new HardwareTimer(TIM6);
HardwareTimer *timer10 = new HardwareTimer(TIM10);
HardwareTimer *timer1 = new HardwareTimer(TIM1);
HardwareTimer *timer7 = new HardwareTimer(TIM7);


void internal_clock_setup();
void Systick_Setup();
void Timer3_Setup();
void Timer4_Setup();
void Timer2_Setup();
void Timer5_Setup();
void Timer6_Setup();
void Timer10_Setup();
void Timer1_Setup(); 
void Timer7_Setup(); 


void Timer_Setup() {    //Main Timer Setup - - - - - - - - - - - - - - - - - - - - - - -

    //Motor Timers
    Timer3_Setup();         //PWM Timer Left        
    Timer4_Setup();         //PWM Timer Right   
    Timer2_Setup();         //Encoder Timer Left
    Timer5_Setup();         //Encoder Timer Right

    //Infrared Timers
    Timer6_Setup(); //Main Interrupt Timer
    

    //Servo1 PWM Timer
    Timer10_Setup();

    //Buzzer PWM and Delay Timer
    Timer1_Setup();
    Timer7_Setup();

    // Enable Systick Timer
    Systick_Setup();

    // Enable the internal clock
    internal_clock_setup();
}


// we need to access the DWT registers to get the current time in microseconds because micros() does not work
void internal_clock_setup(){
    // Enable TRC (Trace Control) and DWT
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // Reset the cycle counter
    DWT->CYCCNT = 0;
    // Enable the cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}


void Systick_Setup(void) {  //Systick Timer Setup
    timer14->setPrescaleFactor(84);          //set prescaler so that 1 tick equals 1us
    timer14->setOverflow(4000);                 // Set overflow to 4000 = 4 ms intervals
    timer14->attachInterrupt(Systick_Interrupt);  
    timer14->refresh();
    timer14->pause();
}



void Timer3_Setup() {   // Motor PWM Left
    timer3->setMode(1, TIMER_OUTPUT_COMPARE_PWM2, MOTOR_LEFT_PWM_1);
    timer3->setMode(2, TIMER_OUTPUT_COMPARE_PWM2, MOTOR_LEFT_PWM_2);
    timer3->setPrescaleFactor(8);
    timer3->setOverflow(1000);
    timer3->setCaptureCompare(1, 0, TICK_COMPARE_FORMAT);           
    timer3->setCaptureCompare(2, 0, TICK_COMPARE_FORMAT);
    timer3->refresh();
    timer3->resume();
}



// Timer 4 setup for motor PWM Right
void Timer4_Setup() {   // Motor PWM Right
    timer4->setMode(2, TIMER_OUTPUT_COMPARE_PWM2, MOTOR_RIGHT_PWM_1);
    timer4->setMode(1, TIMER_OUTPUT_COMPARE_PWM2, MOTOR_RIGHT_PWM_2);
    timer4->setPrescaleFactor(8);
    timer4->setOverflow(1000);
    timer4->setCaptureCompare(1, 0, TICK_COMPARE_FORMAT);           
    timer4->setCaptureCompare(2, 0, TICK_COMPARE_FORMAT);
    timer4->refresh();
    timer4->resume();
}


void Timer2_Setup(void) {                // Konfiguration für PB3 und PA15
    // Takt für GPIOA, GPIOB und Timer 2 aktivieren
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // GPIOB und GPIOA Pins als Alternate Function konfigurieren
    GPIOB->MODER &= ~(3 << 6);  // Bits 6 und 7 löschen (PB3)
    GPIOB->MODER |= (2 << 6);   // Bits 6 und 7 auf '10' setzen (alternativer Funktionsmodus)

    GPIOA->MODER &= ~(3 << 30); // Bits 30 und 31 löschen (PA15)
    GPIOA->MODER |= (2 << 30);  // Bits 30 und 31 auf '10' setzen (alternativer Funktionsmodus)

    // Setzen der alternativen Funktion auf AF1 (TIM2) für PB3 und PA15
    GPIOB->AFR[0] |= (1 << 12); // PB3 AF1 (TIM2)
    GPIOA->AFR[1] |= (1 << 28); // PA15 AF1 (TIM2)

    // Timer 2 konfigurieren
    TIM2->ARR = 4294967295; // Setze den Auto-Reload-Wert auf maximalen Wert

    // Capture/Compare Mode Register konfigurieren für Timer 2
    TIM2->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0); // CC1S und CC2S auf Eingangsmodus (TI1 und TI2) setzen

    // Capture/Compare Enable Register konfigurieren für Timer 2
    TIM2->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P); // Eingänge nicht invertieren

    // Slave Mode Control Register konfigurieren für Timer 2
    TIM2->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; // Timer 2 im Encoder-Modus 3 konfigurieren

    // Timer 2 aktivieren
    TIM2->CR1 |= TIM_CR1_CEN; // Timer 2 starten
}



void Timer5_Setup(void) {                // Konfiguration für PA0 und PA1
    // Takt für GPIOA und Timer 5 aktivieren
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    // GPIOA Pins als Alternate Function konfigurieren
    GPIOA->MODER &= ~(3 << 0);  // Bits 0 und 1 löschen (PA0)
    GPIOA->MODER |= (2 << 0);   // Bits 0 und 1 auf '10' setzen (alternativer Funktionsmodus)

    GPIOA->MODER &= ~(3 << 2);  // Bits 2 und 3 löschen (PA1)
    GPIOA->MODER |= (2 << 2);   // Bits 2 und 3 auf '10' setzen (alternativer Funktionsmodus)

    // Setzen der alternativen Funktion auf AF2 (TIM5) für PA0 und PA1
    GPIOA->AFR[0] |= (2 << 0);  // PA0 AF2 (TIM5)
    GPIOA->AFR[0] |= (2 << 4);  // PA1 AF2 (TIM5)

    // Timer 5 konfigurieren
    TIM5->ARR = 4294967295; // Setze den Auto-Reload-Wert auf maximalen Wert

    // Capture/Compare Mode Register konfigurieren für Timer 5
    TIM5->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0); // CC1S und CC2S auf Eingangsmodus (TI1 und TI2) setzen

    // Capture/Compare Enable Register konfigurieren für Timer 5
    TIM5->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P); // Eingänge nicht invertieren

    // Slave Mode Control Register konfigurieren für Timer 5
    TIM5->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; // Timer 5 im Encoder-Modus 3 konfigurieren

    // Timer 5 aktivieren
    TIM5->CR1 |= TIM_CR1_CEN; // Timer 5 starten
}



void Timer6_Setup(void) {   //Main Infrared Sensor Interrupt Timer
    timer6->setPrescaleFactor(50);               // Set prescaler to 50
    timer6->setOverflow(160);                     // Set overflow to 16 = 50us intervals
    timer6->attachInterrupt(Timer6_Interrupt);
    // timer6->refresh();
    timer6->pause();
}


void Timer10_Setup() {      //Servo1 PWM TImer
    // Takt für GPIOB und Timer 10 aktivieren
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

    // GPIOB Pin PB8 als Alternate Function konfigurieren
    GPIOB->MODER &= ~(3 << (8 * 2));    // Lösche die aktuellen Modi für PB8
    GPIOB->MODER |= (2 << (8 * 2));     // Setze PB8 auf Alternate Function Modus
    GPIOB->AFR[1] &= ~(0xF << ((8 - 8) * 4)); // Lösche die aktuellen Alternate Function Einstellungen für PB8
    GPIOB->AFR[1] |= (3 << ((8 - 8) * 4));    // Setze Alternate Function 3 (AF3) für PB8

    // Timer 10 konfigurieren
    TIM10->PSC = 168 - 1;           // Setze den Prescaler auf 168 (PSC Wert ist Prescaler - 1) -> 1 Tick / uS
    TIM10->ARR = 20000;             // Setze den Auto-Reload-Wert auf 2000
    TIM10->CCR1 = 0;  // Setze den Capture/Compare-Wert für Kanal 1 auf 130 Grad initial

    // PWM Mode 1 für Kanal 1 konfigurieren
    TIM10->CCMR1 &= ~TIM_CCMR1_OC1M;          // Lösche die OC1M Bits
    TIM10->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);// Setze PWM Mode 1 (110) für OC1M Bits
    TIM10->CCMR1 |= TIM_CCMR1_OC1PE;          // Output Compare 1 preload enable

    // Capture/Compare 1 Output Enable
    TIM10->CCER |= TIM_CCER_CC1E;

    // Timer 10 starten
    TIM10->CR1 |= TIM_CR1_CEN;    // Timer starten
}


void Timer1_Setup() {       //Buzzer PWM TImer
    timer1->setMode(4, TIMER_OUTPUT_COMPARE_PWM1, BUZZER);
    timer1->setPrescaleFactor(168);    // Set prescaler so that 1 tick equals 1us
    //freq = 1/T = 1/(Overflow*1us)
    //Overflow = 1/(freq*1us) = 100000/freq
    int freq = 2000;
    timer1->setOverflow(1000000/freq);
    timer1->setCaptureCompare(4, 0, PERCENT_COMPARE_FORMAT);
    timer1->refresh();
    timer1->pause();
}


void Timer7_Setup(void) {   //Buzzer Delay Timer
    timer7->setPrescaleFactor(84000);               // Set prescaler so that 1 tick equals 1ms
    timer7->setOverflow(100);                       // Set overflow to 100 = 100ms intervals
    timer7->attachInterrupt(Timer7_Interrupt);
    timer7->refresh();
    timer7->pause();
}




// ----------------------------------------------------------------
// Interrupt Priorities 
void setInterruptPriority(IRQn_Type irq, uint32_t preemptPriority, uint32_t subPriority) {
    HAL_NVIC_SetPriority(irq, preemptPriority, subPriority);
}

void set_Interrupt_Priority() {

    setInterruptPriority(EXTI15_10_IRQn, 0, 1);     //UI_Button und Encoder B
    setInterruptPriority(EXTI0_IRQn, 9, 1);         //Encoder A
    setInterruptPriority(EXTI2_IRQn, 0, 1);         //Motor Fehler

    timer2->setInterruptPriority(1,1);              //Motor Encoder links
    timer5->setInterruptPriority(1,2);              //Motor Encoder rechts
    timer6->setInterruptPriority(5,1);              //Main Infrared Sensor
    timer14->setInterruptPriority(7,1);             //Systick
}