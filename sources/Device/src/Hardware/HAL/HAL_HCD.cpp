// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>
#include <usbh_core.h>


static HCD_HandleTypeDef handle;


void HAL_HCD::Init()
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
    isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    isGPIO.Alternate = GPIO_AF10_OTG_FS;

    HAL_GPIO_Init(GPIOA, &isGPIO);

    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);

    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}


void HAL_HCD::IRQHandler()
{
    HAL_HCD_IRQHandler(&handle);
}


int HAL_HCD::USBH_LL_Init(void *usbh_HandleTypeDef)
{
    USBH_HandleTypeDef *phost = (USBH_HandleTypeDef *)usbh_HandleTypeDef;

    /* Set the LL driver parameters */
    handle.Instance = USB_OTG_FS;
    handle.Init.speed = HCD_SPEED_FULL;
    handle.Init.Host_channels = 11;
    handle.Init.dma_enable = 0;
    handle.Init.low_power_enable = 0;
    handle.Init.phy_itface = HCD_PHY_EMBEDDED;
    handle.Init.Sof_enable = 0;
    handle.Init.vbus_sensing_enable = 0;
    handle.Init.use_external_vbus = 0;

    /* Link the driver to the stack */
    handle.pData = phost;
    phost->pData = &handle;
    /* Initialize the LL driver */
    HAL_HCD_Init(&handle);

    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&handle));

    return (int)USBH_OK;
}


int HAL_HCD::USBH_LL_SetToggle(uint8 pipe, uint8 toggle)
{
    if(handle.hc[pipe].ep_is_in)
    {
        handle.hc[pipe].toggle_in = toggle;
    }
    else
    {
        handle.hc[pipe].toggle_out = toggle;
    }
    return USBH_OK;
}


uint8 HAL_HCD::USBH_LL_GetToggle(uint8 pipe)
{
    uint8_t toggle = 0;

    if(handle.hc[pipe].ep_is_in)
    {
        toggle = handle.hc[pipe].toggle_in;
    }
    else
    {
        toggle = handle.hc[pipe].toggle_out;
    }
    return toggle;

}
