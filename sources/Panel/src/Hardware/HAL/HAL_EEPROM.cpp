#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"
#include <stm32f4xx_hal.h>


// \todo Занести это в структуру и обращаться только через структуру, чтобы исключить или уменьшить ошибки в функциях вроде GetSector  

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




// Возвращает первый адрес, значение в котором равно 0xffffffff (можно записывать). Поиск начинается с адреса start, продолжается в участке памяти размером sizeFull. Кратно размеру sizeObject
static uint FindFirstFreeRecord(uint start, uint sizeSector, uint sizeRecord);
// Возвращает адрес последнего блока, в котором первый байт не равен 0xffffffff (в нём сохраенены последние настройки)
static uint FindLastOccupiedRecord(uint start, uint sizeSector, uint sizeRecord);
// Стирает сектор с начальным адресом startAddress
static void EraseSector(uint startAddress);
// Записывает size байт из массива data по адресу address
static void WriteData(uint address, void *data, uint size);
// Возвращает системный идентификатор сектора с начальным адресом address. Ежели такового нету, возвращает -1
static uint GetSector(uint address);



void HAL_EEPROM::SaveSettings(CalibrationSettings *settings)
{
    uint address = FindFirstFreeRecord(ADDR_SECTOR_CALIBRATION, SIZE_SECTOR_CALIBRATION, sizeof(CalibrationSettings));

    if (address == 0)
    {
        EraseSector(ADDR_SECTOR_CALIBRATION);
        address = ADDR_SECTOR_CALIBRATION;
    }

    WriteData(address, settings, sizeof(CalibrationSettings));
}


void HAL_EEPROM::LoadSettings(CalibrationSettings *settings)
{
    uint address = FindLastOccupiedRecord(ADDR_SECTOR_CALIBRATION, SIZE_SECTOR_CALIBRATION, sizeof(CalibrationSettings));

    if (address)                                            // Если нашли сохранённую запись
    {
        *settings = *(reinterpret_cast<CalibrationSettings *>(address));      // То запишем её в целевой объект
    }
}


static uint FindFirstFreeRecord(uint start, uint sizeFull, uint sizeRecord)
{
    uint address = start;
    uint end = start + sizeFull;

    while (address < end)
    {
        if (*(reinterpret_cast<uint *>(address)) == 0xffffffffU)
        {
            return address;
        }
        address += sizeRecord;
    }

    return 0;
}


static uint FindLastOccupiedRecord(uint start, uint sizeSector, uint sizeRecord)
{
    uint address = FindFirstFreeRecord(start, sizeSector, sizeRecord);

    if (address == 0)                               // Если свободной записи нет, значит, весь сектора заполнен
    {
        return start + sizeSector - sizeRecord;     // надо считвать последнюю запись
    }

    if (address == start)                           // Если первая свободная запись находится в начале сектора, то сектор пуст - запись в него не производилась
    {
        return 0;                                   // Возвращаем 0 как признак того, что записей нет
    }

    return address - sizeRecord;                    // Во всех остальных случаях возвращаем адрес записи, предыдущей по отношению к первой свободной
}


static void EraseSector(uint startAddress)
{
    if (GetSector(startAddress) == static_cast<uint>(-1))
    {
        return;
    }

    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef isFLASH;
    isFLASH.TypeErase = TYPEERASE_SECTORS;
    isFLASH.Sector = GetSector(startAddress);
    isFLASH.NbSectors = 1;
    isFLASH.VoltageRange = VOLTAGE_RANGE_3;

    uint error = 0;

    HAL_FLASHEx_Erase(&isFLASH, &error);

    HAL_FLASH_Lock();
}


static uint GetSector(uint address)
{
    struct StructSector
    {
        uint number;
        uint startAddress;
    };

    static const StructSector sectors[] =
    {
        {FLASH_SECTOR_11, ADDR_SECTOR_CALIBRATION},
        {}
    };

    int i = 0;
    while (sectors[i].startAddress)
    {
        if (sectors[i].startAddress == address)
        {
            return sectors[i].number;
        }
        i++;
    }

    return static_cast<uint>(-1);
}


static void WriteData(uint address, void *data, uint size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    for (uint i = 0; i < size; i++)
    {
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address++, (static_cast<uint8 *>(data))[i]);
    }

    HAL_FLASH_Lock();
}
