// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static I2C_HandleTypeDef handle =
{
    I2C1,
    {
        100000,                     // ClockSpeed
        I2C_DUTYCYCLE_2,            // DutyCycle
        0,                          // OwnAddress1
        I2C_ADDRESSINGMODE_7BIT,    // AddressingMode
        I2C_DUALADDRESS_DISABLE,    // DualAddressMode
        0,                          // OwnAddress2
        I2C_GENERALCALL_DISABLE,    // GeneralCallMode
        I2C_NOSTRETCH_DISABLE       // NoStretchMode
    },
    0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_I2C_STATE_RESET, HAL_I2C_MODE_NONE, 0, 0, 0, 0, 0
};


void HAL_I2C1::Init()
{
    GPIO_InitTypeDef isGPIO =
    {//    SCL          SDA
        GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_OD,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF4_I2C1
    };
    HAL_GPIO_Init(GPIOB, &isGPIO);

    HAL_I2C_Init(&handle);
}


void HAL_I2C1::Transmit(uint8 address, uint8 data[3])
{
    // Смещение на один бит влево - страшная штука. Если не знать, можно потерять много времени
    HAL_I2C_Master_Transmit(&handle, (uint16)(address << 1), data, 3, 100);
}
