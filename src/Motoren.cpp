#include <iostream>
#include <vector>
#include "Arduino.h"
#include "wiring.h"
#include "Pin_Init.h"

class motors
{
private:
    /* data */
public:
    boolean bR_A_flag, bR_B_flag, bL_A_flag, bL_B_flag;
    int iL_A_count, iL_B_count, iR_A_count, iR_B_count;

    void setup()
    {
        attachInterrupt(PA15, isr_int_L_A, CHANGE);     /*Interrupts nochmal prüfen*/
        attachInterrupt(PB3, isr_int_L_B, CHANGE);
        attachInterrupt(PA0, isr_int_R_A, CHANGE);
        attachInterrupt(PA1, isr_int_R_B, CHANGE);
        NVIC_SetPriority(EXTI1_IRQn, 0);
        NVIC_SetPriority(EXTI0_IRQn, 0);
        NVIC_SetPriority(EXTI3_IRQn, 0);
        NVIC_SetPriority(EXTI15_10_IRQn, 0);            /*PIN Belegung nochmal prüfen*/
    }

    void isr_int_L_A()      //interrupt-service-routine_ENC_A_links
    {
        iL_A_count++;
        bL_A_flag = true;
    }

    void isr_int_L_B()      //interrupt-service-routine_ENC_B_links
    {
        iL_B_count++;
        bL_B_flag = true;
    }

     void isr_int_R_A()      //interrupt-service-routine_ENC_A_rechts
    {
        iR_A_count++;
        bR_A_flag = true;
    }

     void isr_int_R_B()      //interrupt-service-routine_ENC_B_rechts
    {
        iR_B_count++;
        bR_B_flag = true;
    }







    motors(/* args */);
    ~motors();
};