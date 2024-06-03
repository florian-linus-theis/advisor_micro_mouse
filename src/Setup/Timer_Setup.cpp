#include "Setup/Setup.h"

//unused Timers 7,8,9,11,12,13
HardwareTimer *timer14 = new HardwareTimer(TIM14);
HardwareTimer *timer3 = new HardwareTimer(TIM3);
HardwareTimer *timer4 = new HardwareTimer(TIM4);
HardwareTimer *timer2 = new HardwareTimer(TIM2);
HardwareTimer *timer5 = new HardwareTimer(TIM5);
HardwareTimer *timer6 = new HardwareTimer(TIM6);
HardwareTimer *timer10 = new HardwareTimer(TIM10);
HardwareTimer *timer1 = new HardwareTimer(TIM1);



void Systick_Setup();
void Timer3_Setup();
void Timer4_Setup_Motor();
void Timer4_Setup_Servo();
void Timer2_Setup();
void Timer5_Setup();
void Timer6_Setup();
void Timer10_Setup();
void Timer1_Setup(); 


void Timer_Setup() {    //Main Timer Setup - - - - - - - - - - - - - - - - - - - - - - -
    // Enable Systick Timer
    Systick_Setup();

    //Motor Timers
    Timer3_Setup(); //PWM Timer Left        
    Timer4_Setup_Motor(); //PWM Timer Right 
    // Timer4_Setup_Servo(); //PWM Timer Servo   
    Timer2_Setup(); //Encoder Timer Left    //TO-DO / Overhaul
    Timer5_Setup(); //Encoder Timer Right   //TO-DO / Overhaul

    //Infrared Timers
    Timer6_Setup(); //Main Interrupt Timer
    

    //Servo1 PWM Timer
    Timer10_Setup();                        //TO-DO

    //Buzzer PWM Timer
    Timer1_Setup();                         //TO-DO
}




void Systick_Setup(void) {  //Systick Timer Setup
    timer14->setPrescaleFactor(40);          // Set prescaler to 40 
    timer14->setOverflow(4200);                 // Set overflow to 4200 = 2 ms intervals
    timer14->attachInterrupt(Systick_Interrupt);  
    timer14->refresh();
    timer14->pause();
}



void Timer3_Setup() {   // Motor PWM Left
    timer3->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, MOTOR_LEFT_PWM_1);
    timer3->setMode(2, TIMER_OUTPUT_COMPARE_PWM1, MOTOR_LEFT_PWM_2);
    timer3->setPrescaleFactor(8);
    timer3->setOverflow(1000);
    timer3->setCaptureCompare(1, 0, TICK_COMPARE_FORMAT);           
    timer3->setCaptureCompare(2, 0, TICK_COMPARE_FORMAT);
    timer3->refresh();
    timer3->resume();
}


// Timer4 calculations
const uint16_t PRESCALER = 1680 - 1; // Prescaler to get 100 kHz timer frequency
const uint16_t PERIOD = 2000 - 1; // Period to get 50 Hz PWM frequency

// Timer 4 setup for motor PWM Right
void Timer4_Setup_Motor() {   // Motor PWM Right
    timer4->pause(); // Pause the timer while (re-)configuring
    timer4->setMode(2, TIMER_OUTPUT_COMPARE_PWM1, MOTOR_RIGHT_PWM_1);
    timer4->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, MOTOR_RIGHT_PWM_2);
    timer4->setPrescaleFactor(8);
    timer4->setOverflow(1000);
    timer4->setCaptureCompare(1, 0, TICK_COMPARE_FORMAT);           
    timer4->setCaptureCompare(2, 0, TICK_COMPARE_FORMAT);
    timer4->refresh();
    timer4->resume();
}

// Timer 4 setup for servo ballgrabber
void Timer4_Setup_Servo() {      //Servo1 PWM TImer
    timer4->pause(); // Pause the timer while (re-)configuring
    timer4->setMode(3, TIMER_OUTPUT_COMPARE_PWM1, SERVO_PWM_1); // Channel 1 for PB8
    timer4->setPrescaleFactor(PRESCALER); // Set prescaler to divide by 1680
    timer4->setOverflow(PERIOD); // Set overflow to 2000 for 50 Hz PWM
    // Configure PWM mode on channel 3
    timer4->setCaptureCompare(3, 500, MICROSEC_COMPARE_FORMAT); // 500 is Initial pulse width for having it directly in front of mouse (saves time)
    // Refresh and start the timer
    timer4->refresh();
    timer4->resume(); // Start the timer
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
    timer6->setOverflow(16);                     // Set overflow to 16 = 50us intervals
    timer6->attachInterrupt(Timer6_Interrupt);
    timer6->refresh();
    timer6->pause();
}


void Timer10_Setup() {      //Servo1 PWM TImer
    // timer10->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, SERVO_PWM_1); -- broken as fuck
    timer10->setPrescaleFactor(26); // Set prescaler to 26
    timer10->setOverflow(64615); // Set overflow to 64615
    timer10->setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    timer10->refresh();
    timer10->resume();
}


void Timer1_Setup() {       //Buzzer PWM TImer
    timer1->setMode(4, TIMER_OUTPUT_COMPARE_PWM1, BUZZER);
    timer1->setPrescaleFactor(84);    // Set prescaler so that 1 tick equals 1us
    //freq = 1/T = 1/(Overflow*1us)
    //Overflow = 1/(freq*1us) = 100000/freq
    int freq = 2000;
    timer1->setOverflow(1000000/freq);
    timer1->setCaptureCompare(4, 0, PERCENT_COMPARE_FORMAT);
    timer1->refresh();
    timer1->pause();
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