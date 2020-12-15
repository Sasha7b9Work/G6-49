#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>
#include <cstdlib>
#include <cstring>


static CRC_HandleTypeDef handleCRC = { CRC }; //-V2571


void HAL_CRC32::Init()
{
    if(HAL_CRC_Init(&handleCRC) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


uint HAL_CRC32::Calculate(const void *data, uint size)
{
    uint sizeBuffer = size;
    while(sizeBuffer % 4)                           // Увеличиваем до ближайшего кратного четырём
    {
        ++sizeBuffer;
    }

    uint result = 0;

    uint *buffer = static_cast<uint *>(std::malloc(sizeBuffer));    // Выделяем память для нового буфера //-V2511

    if(buffer)
    {
        std::memcpy(buffer, data, size);                            // Копируем данные в новый буфер

        for(uint i = size; i < sizeBuffer; i++)                    // Заполняем оставшееся место нулями
        {
            buffer[i] = 0; //-V2563
        }

        result = HAL_CRC_Calculate(&handleCRC, buffer, sizeBuffer / 4); //-V2571
    }

    std::free(buffer); //-V2511

    return result;
}
