#include "stdafx.h"
#ifndef WIN32
#include "CPU.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PCD_HandleTypeDef CPU::PCD::handle;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::PCD::Init()
{
    __GPIOB_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_14 | GPIO_PIN_15,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF12_OTG_HS_FS
    };

    HAL_GPIO_Init(GPIOB, &isGPIO);

    isGPIO.Pin = GPIO_PIN_13;
    isGPIO.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &isGPIO);

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 6, 1);

    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}
