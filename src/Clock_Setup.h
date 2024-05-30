#include "Setup.h"

/*
void Clock_Setup() {
    //more to come
    //set Core Freq to 168MHz
    //scale Peripheral Clocks accordingly

    //Update Timers after !!!

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure the main internal regulator output voltage
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure.
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    // Initializes the CPU, AHB and APB buses clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}
*/


void Clock_Setup() {
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // Set voltage scaling to Scale 1 mode
    PWR->CR |= PWR_CR_VOS;

    // Enable HSI oscillator
    RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0);

    // Configure the PLL
    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSI |  // HSI oscillator is the PLL source
                    (8 << RCC_PLLCFGR_PLLM_Pos) |  // PLLM = 8
                    (168 << RCC_PLLCFGR_PLLN_Pos) |  // PLLN = 168
                    (0 << RCC_PLLCFGR_PLLP_Pos) |  // PLLP = 2 (00 -> PLLP = 2)
                    (4 << RCC_PLLCFGR_PLLQ_Pos);  // PLLQ = 4

    // Enable the PLL
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    // Configure Flash prefetch, Instruction cache, Data cache and wait state
    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;

    // Select the PLL as system clock source
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // Set HCLK (AHB) prescaler
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    // Set PCLK1 (APB1) prescaler
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    // Set PCLK2 (APB2) prescaler
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV4;
}