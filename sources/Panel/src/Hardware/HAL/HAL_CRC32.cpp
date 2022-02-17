#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>
#include <cstdlib>
#include <cstring>


static CRC_HandleTypeDef handleCRC = { CRC };


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
    while(sizeBuffer % 4)                           // ����������� �� ���������� �������� ������
    {
        ++sizeBuffer;
    }

    uint result = 0;

    uint *buffer = static_cast<uint *>(std::malloc(sizeBuffer));    // �������� ������ ��� ������ ������

    if(buffer)
    {
        std::memcpy(buffer, data, size);                            // �������� ������ � ����� �����

        for(uint i = size; i < sizeBuffer; i++)                    // ��������� ���������� ����� ������
        {
            buffer[i] = 0;
        }

        result = HAL_CRC_Calculate(&handleCRC, buffer, sizeBuffer / 4);
    }

    std::free(buffer);

    return result;
}
