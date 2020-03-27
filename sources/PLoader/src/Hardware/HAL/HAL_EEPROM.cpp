#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"
#include <stm32f4xx_hal.h>


/// \todo Занести это в структуру и обращаться только через структуру, чтобы исключить или уменьшить ошибки в функциях вроде GetSector  

#define ADDR_SECTOR_0   ((uint)0x08000000)  // 16k  Загрузчик
#define ADDR_SECTOR_1   ((uint)0x08004000)  // 16k
#define ADDR_SECTOR_2   ((uint)0x08008000)  // 16k
#define ADDR_SECTOR_3   ((uint)0x0800c000)  // 16k
#define ADDR_SECTOR_4   ((uint)0x08010000)  // 64k  SettingsCalibration
#define ADDR_SECTOR_5   ((uint)0x08020000)  // 128k Основная прошивка 1
#define ADDR_SECTOR_6   ((uint)0x08040000)  // 128k Основная прошивка 2
#define ADDR_SECTOR_7   ((uint)0x08060000)  // 128k Основная прошивка 3
#define ADDR_SECTOR_8   ((uint)0x08080000)  // 128k
#define ADDR_SECTOR_9   ((uint)0x080a0000)  // 128k
#define ADDR_SECTOR_10  ((uint)0x080c0000)  // 128k
#define ADDR_SECTOR_11  ((uint)0x080e0000)  // 128k
#define ADDR_SECTOR_CALIBRATION ADDR_SECTOR_11
#define SIZE_SECTOR_CALIBRATION (128 * 1024)

#define CLEAR_FLASH_FLAGS \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    |  /* end of operation flag              */   \
                           FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                           FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                           FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                           FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                           FLASH_FLAG_PGSERR);  /* programming sequence error flag    */


void HAL_EEPROM::SaveSettings()
{
}


void HAL_EEPROM::LoadSettings()
{
}
