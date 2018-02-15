#include "defines.h"
#include "stm32f4xx_hal.h"
#include <stm32f429xx.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL_MspInit(void)
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
    if(hltdc->Instance == LTDC)
    {
        __HAL_RCC_LTDC_CLK_ENABLE();
        __HAL_RCC_DMA2D_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();

        GPIO_InitTypeDef isGPIO = 
        {
            //  R3         R6
            GPIO_PIN_0 | GPIO_PIN_1,
            GPIO_MODE_AF_PP,
            GPIO_NOPULL,
            GPIO_SPEED_FREQ_LOW,
            GPIO_AF9_LTDC
        };
        HAL_GPIO_Init(GPIOB, &isGPIO);

        //              B5          VSYNC         G2            R4          R5
        isGPIO.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_11 | GPIO_PIN_12;
        isGPIO.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOA, &isGPIO);

        //              G4             G5            B6          B7
        isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_8 | GPIO_PIN_9;
        HAL_GPIO_Init(GPIOB, &isGPIO);

        //              HSYNC         G6           R2
        isGPIO.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
        HAL_GPIO_Init(GPIOC, &isGPIO);

        //                B3          G7          B2
        isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_3 | GPIO_PIN_6;
        HAL_GPIO_Init(GPIOD, &isGPIO);

        //               G3             B4           DE            CLK           R7
        isGPIO.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        HAL_GPIO_Init(GPIOE, &isGPIO);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
    if(htim_pwm->Instance == TIM4)
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
    if(htim->Instance == TIM4)
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
    if(htim_pwm->Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_DISABLE();
    }

}
