#include "stdafx.h"
#ifndef WIN32
#include "CPU.h"
#endif



    HCD_HandleTypeDef CPU::HCD::handle;



void CPU::HCD::Init()
{
    GPIO_InitTypeDef isGPIO =
    {
        0,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        0
    };

    /*
    104 - PA12 - D+
    103 - PA11 - D-
    101 - PA9  - VBUS
    */

    __GPIOA_CLK_ENABLE();
    __USB_OTG_FS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    isGPIO.Speed = GPIO_SPEED_HIGH;
    isGPIO.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    isGPIO.Alternate = GPIO_AF10_OTG_FS;

    HAL_GPIO_Init(GPIOA, &isGPIO);

    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);

    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}
