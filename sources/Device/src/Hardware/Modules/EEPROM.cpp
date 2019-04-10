#include "stdafx.h"
#ifndef WIN32
#include "log.h"
#include "EEPROM.h"
#include "Generator/FPGA.h"
#include "Hardware/Timer.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define SECTOR_0                    ((uint)0x08000000)          // 16k  Загрузчик
#define SECTOR_1                    ((uint)0x08004000)          // 16k
#define SECTOR_2                    ((uint)0x08008000)          // 16k
#define SECTOR_3                    ((uint)0x0800c000)          // 16k
#define SECTOR_CALIBRATION_4        ((uint)0x08010000)          // 64k  SettingsCalibration
#define SIZE_SECTOR_CALIBRATION     (64 * 1024)
#define SECTOR_5                    ((uint)0x08020000)          // 128k Основная прошивка 1
#define SECTOR_6                    ((uint)0x08040000)          // 128k Основная прошивка 2
#define SECTOR_7                    ((uint)0x08060000)          // 128k Основная прошивка 3
#define SECTOR_8                    ((uint)0x08080000)          // 128k
#define SECTOR_9                    ((uint)0x080a0000)          // 128k
#define SECTOR_TEMP_10              ((uint)0x080c0000)          // 128k
#define SIZE_SECTOR_TEMP            (128 * 1024)
#define SECTOR_SIGNAL_FPGA_11       ((uint)0x080e0000)          // Сигнал с флешки здесь записан
#define SIZE_SECTOR_SIGNAL_FPGA     (128 * 1024)

#define CLEAR_FLASH_FLAGS \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    |  /* end of operation flag              */   \
                           FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                           FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                           FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                           FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                           FLASH_FLAG_PGSERR);  /* programming sequence error flag    */



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Возвращает первый адрес, значение в котором равно 0xffffffff (можно записывать). Поиск начинается с адреса start, продолжается в участке памяти размером sizeFull. Кратно размеру sizeObject
static uint FindFirstFreeRecord(uint start, uint sizeSector, uint sizeRecord);
/// Возвращает адрес последнего блока, в котором первый байт не равен 0xffffffff (в нём сохраенены последние настройки)
static uint FindLastOccupiedRecord(uint start, uint sizeSector, uint sizeRecord);
/// Стирает сектор с начальным адресом startAddress
static void EraseSector(uint startAddress);
/// Записывает size байт из массива data по адресу address
static void WriteData(uint dest, void *src, uint size);
/// Возвращает системный идентификатор сектора с начальным адресом address. Ежели такового нету, возвращает -1
static uint GetSector(uint address);
/// Возвращает размер сектора с данным начальным адресом
static uint SizeSector(uint address);
/// Возвращает адрес EEPROM, куда надо сохранять данные этого канала
static uint AddressForData(Chan::E ch);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EEPROM::SaveSettings(CalibrationSettings *settings)
{
    uint address = FindFirstFreeRecord(SECTOR_CALIBRATION_4, SizeSector(SECTOR_CALIBRATION_4), sizeof(CalibrationSettings));

    if (address == 0)
    {
        EraseSector(SECTOR_CALIBRATION_4);
        address = SECTOR_CALIBRATION_4;
    }

    WriteData(address, settings, sizeof(CalibrationSettings));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint SizeSector(uint address)
{
    if (GetSector(address) < 4)       { return (16 * 1024); }
    else if (GetSector(address) == 4) { return (64 * 1024); }
    else
    {
        // здесь ничего
    }

    return (128 * 1024);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::LoadSettings(CalibrationSettings *settings)
{
    uint address = FindLastOccupiedRecord(SECTOR_CALIBRATION_4, SizeSector(SECTOR_CALIBRATION_4), sizeof(CalibrationSettings));

    if (address)                                            // Если нашли сохранённую запись
    {
        *settings = *((CalibrationSettings *)address);      // То запишем её в целевой объект
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint FindFirstFreeRecord(uint start, uint sizeFull, uint sizeRecord)
{
    uint address = start;
    uint end = start + sizeFull;

    while (address < end)
    {
        if (*(uint *)address == 0xffffffffU)
        {
            return address;
        }
        address += sizeRecord;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void EraseSector(uint startAddress)
{
    if (GetSector(startAddress) == (uint)-1)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint GetSector(uint address)
{
    struct StructSector
    {
        uint number;
        uint startAddress;
    };

    static const StructSector sectors[] =
    {
        {FLASH_SECTOR_4,  SECTOR_CALIBRATION_4},
        {FLASH_SECTOR_10, SECTOR_TEMP_10},
        {FLASH_SECTOR_11, SECTOR_SIGNAL_FPGA_11},
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

    return (uint)(-1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteData(uint dest, void *src, uint size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    for (uint i = 0; i < size; i++)
    {
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, dest++, ((uint8 *)src)[i]);
    }

    HAL_FLASH_Lock();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::SaveSignal(Chan::E ch, float *data)
{
    uint sizeData = FPGA::NUM_POINTS * sizeof(float);

    EraseSector(SECTOR_TEMP_10);                                                        // Обнуляем сектор для временных данных для временного сохранения тех данных, которые не нужно перезаписывать
    WriteData(SECTOR_TEMP_10, (void *)SECTOR_SIGNAL_FPGA_11, sizeData * Chan::Number);  // Сохраняем существующие данные //-V566
    EraseSector(SECTOR_SIGNAL_FPGA_11);                                                 // Стираем сектор для хранения данных
    WriteData(AddressForData(ch), data, sizeData);                                      // Записываем данные канала

    ch = (ch == Chan::A) ? Chan::B : Chan::A;

    data = (float *)SECTOR_TEMP_10; //-V566

    if (ch == Chan::B)
    {
        data += FPGA::NUM_POINTS;                                                        // Вычисляем указатель на данные канала, который не требуется перезаписывать
    }

    WriteData(AddressForData(ch), data, sizeData);                                      // И сохраняем их
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint AddressForData(Chan::E ch)
{
    uint result = SECTOR_SIGNAL_FPGA_11;
    if (ch == Chan::B)
    {
        result += FPGA::NUM_POINTS * sizeof(float);
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float *EEPROM::GetSignal(Chan::E ch)
{
    float *result = (float *)(SECTOR_SIGNAL_FPGA_11); //-V566

    if (ch == Chan::B)
    {
        result += FPGA::NUM_POINTS;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::Init()
{
    uint *data = (uint *)SECTOR_SIGNAL_FPGA_11; //-V566

    for (int i = 0; i < (FPGA::NUM_POINTS * Chan::Number); i++)
    {
        if (data[i] != (uint)(-1))
        {
            return;
        }
    }

    EraseSector(SECTOR_SIGNAL_FPGA_11);

    float *address = (float *)SECTOR_SIGNAL_FPGA_11; //-V566

    float value = 0.0f;

    for (int i = 0; i < (FPGA::NUM_POINTS * Chan::Number); i++)
    {
        WriteData((uint)address, &value, 4);
    }
}
