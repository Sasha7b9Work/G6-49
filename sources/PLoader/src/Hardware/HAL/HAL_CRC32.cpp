// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


namespace HAL_CRC32
{
    static CRC_HandleTypeDef handle = { CRC };
}


void HAL_CRC32::Init()
{
    HAL_CRC_Init(&handle);
}


uint HAL_CRC32::Calculate(uint8 *data, uint size)
{
    while ((size % 4) != 0)
    {
        data[size++] = 0;
    }

    return HAL_CRC_Calculate(&handle, (uint *)data, (uint)(size / 4));
}
