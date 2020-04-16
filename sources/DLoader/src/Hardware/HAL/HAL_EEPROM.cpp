#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */


#define ADDRESS_SECTOR_TEMP ((uint)0x080c0000) // Вспомогательный сектор. Сюда записывается прошивка Panel при процедуре обновления


// Стирает count секторов, начиная с сектора start
static void EraseSectors(uint start, uint count);


void HAL_EEPROM::WriteBuffer(int address, uint8 *data, int size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    for(int i = 0; i < size; i++)
    {
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, static_cast<uint>(address), data[i]);
        ++address;
    }

    HAL_FLASH_Lock();
}


void HAL_EEPROM::EraseSectors(int count)
{
    ::EraseSectors(FLASH_SECTOR_5, static_cast<uint>(count));
}


void HAL_EEPROM::EraseSectorTemp()
{
    ::EraseSectors(FLASH_SECTOR_10, 1);
}


static void EraseSectors(uint start, uint count)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef is =
    {
        FLASH_TYPEERASE_SECTORS,
        0,
        start,
        count,
        FLASH_VOLTAGE_RANGE_3
    };

    uint error = 0;

    HAL_FLASHEx_Erase(&is, &error);

    HAL_FLASH_Lock();
}
