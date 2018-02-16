#include "Hardware.h"
#include "Generator/Generator.h"
#include "Interface/Interface.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Hardware hardware;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hardware::Init(void)
{
    SystemClock_Config();
    HAL_Init();
    InitPins();
    GPIOS_Init();
    WritePin(Pin_P3_OutA, false);
    WritePin(Pin_P4_OutB, false);
    timer.PauseOnTime(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    generator.Init();
    interface.Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::GPIOS_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO;

    // На этом пине будем выставлять занятость генератора
    isGPIO.Pin = GPIO_PIN_2;
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIO.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &isGPIO);

    hardware.SetBusy();

    isGPIO.Pin = GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_NOPULL;
    isGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    isGPIO.Alternate = GPIO_AF12_OTG_HS_FS;
    HAL_GPIO_Init(GPIOB, &isGPIO);

    isGPIO.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_NOPULL;
    isGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    isGPIO.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::SetBusy(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::SetReady(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ERROR_HANDLER;
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
        |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        ERROR_HANDLER;
    }

    /**Configure the Systick interrupt time 
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
__attribute((noreturn)) void _Error_Handler(const char *, int)
{
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT

//----------------------------------------------------------------------------------------------------------------------------------------------------
void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif

