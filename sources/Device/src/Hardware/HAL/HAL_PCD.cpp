#include "Hardware/HAL/HAL.h"
#include <usbd_core.h>


static PCD_HandleTypeDef handle;


void HAL_PCD::Init()
{
    __GPIOB_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE();
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

    /*
    isGPIO.Pin = GPIO_PIN_13;
    isGPIO.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &isGPIO);
    */

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 6, 1);

    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}


void HAL_PCD::IRQHandler()
{
    HAL_PCD_IRQHandler(&handle);
}


int HAL_PCD::USBD_LL_Init(void *usbd_HandleTypeDef)
{
    USBD_HandleTypeDef *pdev = static_cast<USBD_HandleTypeDef *>(usbd_HandleTypeDef);

    NVIC_SetPriority(SysTick_IRQn, 0);

    handle.Instance = USB_OTG_HS;
    handle.Init.dev_endpoints = 6;
    handle.Init.speed = PCD_SPEED_FULL;
    handle.Init.dma_enable = DISABLE;
    handle.Init.ep0_mps = 0x40;
    handle.Init.phy_itface = PCD_PHY_EMBEDDED;
    handle.Init.Sof_enable = DISABLE;
    handle.Init.low_power_enable = DISABLE;
    handle.Init.lpm_enable = DISABLE;
    handle.Init.vbus_sensing_enable = ENABLE;
    handle.Init.use_dedicated_ep1 = DISABLE;

    // Link The driver to the stack
    handle.pData = pdev;
    pdev->pData = &handle;
    // Initialize LL Driver
    HAL_PCD_Init(&handle);

    HAL_PCDEx_SetRxFiFo(&handle, 0x80);
    HAL_PCDEx_SetTxFiFo(&handle, 0, 0x40);
    HAL_PCDEx_SetTxFiFo(&handle, 1, 0x80);

    return USBD_OK;
}
