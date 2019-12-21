#include "Hardware/HAL/HAL.h"
static CRC_HandleTypeDef handleCRC = { CRC };



void HAL_CRC32::Init()
{
    if(HAL_CRC_Init(&handleCRC) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


uint HAL_CRC32::Calculate(const uint8 *data, uint size)
{
    uint sizeBuffer = size;
    while(sizeBuffer % 4)                           // Увеличиваем до ближайшего кратного четырём
    {
        ++sizeBuffer;
    }

    uint *buffer = static_cast<uint *>(std::malloc(sizeBuffer));      // Выделяем память для нового буфера

    uint result = 0U;

    if(buffer)
    {
        std::memcpy(buffer, data, size);                     // Копируем данные в новый буфер

        for(uint i = size; i < sizeBuffer; i++)          // Заполняем оставшееся место нулями
        {
            buffer[i] = 0;
        }

        result = HAL_CRC_Calculate(&handleCRC, buffer, sizeBuffer / 4);

        std::free(buffer);
    }

    return result;
}
