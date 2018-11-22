#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "SPI.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static SPI_HandleTypeDef handle =                                   // Для связи с панелью
{
    SPI1,
    {
        SPI_MODE_SLAVE,                 // Init.Mode
        SPI_DIRECTION_2LINES,           // Init.Direction
        SPI_DATASIZE_8BIT,              // Init.DataSize
        SPI_POLARITY_HIGH,              // Init.CLKPolarity
        SPI_PHASE_2EDGE,                // Init.CLKPhase
        SPI_NSS_SOFT,                   // Init.NSS
        SPI_BAUDRATEPRESCALER_16,       // Init.BaudRatePrescaler
        SPI_FIRSTBIT_MSB,               // Init.FirstBit
        SPI_TIMODE_DISABLED,            // Init.TIMode
        SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
        7                               // InitCRCPolynomial
    },
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI1_::Init()
{
    GPIO_InitTypeDef isGPIOA =
    {   //  SCK         MI           MO
        GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_HIGH,
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA);

    HAL_SPI_Init(&handle);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SPI1_::Receive(void *buffer, uint size, uint timeout)
{
    return HAL_SPI_Receive(&handle, (uint8 *)buffer, (uint16)size, timeout) == HAL_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SPI1_::Transmit(void *buffer, uint size, uint timeout)
{
    return HAL_SPI_Transmit(&handle, (uint8 *)buffer, (uint16)size, timeout) == HAL_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SPI1_::Transmit(uint value, uint timeout)
{
    return Transmit(&value, 4, timeout);
}
