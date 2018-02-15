#include "FLASH.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */

#define ADDR_SECTOR_SETTINGS    ((uint)0x080E0000)
#define SIZE_SECTOR_SETTINGS    (128 * 1024)
#define SIZE_RECORD             (4 * 1024)          ///< Максиммальный размер сохраняемых настроек

#define READ_HALF_WORD(address) (*((volatile uint16 *)address))


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool EraseSector(uint startAdddress);
static void WriteBufferBytes(uint address, void *buffer, int numBytes);
static uint GetSector(uint startAddress);
static void ReadBufferBytes(uint addrSrc, void *bufferDest, int size);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FLASH_SaveSettings(void)
{
    // Записываем в Settings.size текущий размер структуры Settings
    set.size = sizeof(Settings);

    // Находим первый свободный адрес записи
    int address = ADDR_SECTOR_SETTINGS;
    while (READ_HALF_WORD(address) != 0xffff &&                             // Пока по адресу, кратному SIZE_RECORD, записаны настройки
           (uint)address < (ADDR_SECTOR_SETTINGS + SIZE_SECTOR_SETTINGS))   // и мы не вышли за пределы секторы настроек
    {
        address += SIZE_RECORD;
    }

    if (address == ADDR_SECTOR_SETTINGS + SIZE_SECTOR_SETTINGS)     // Если адрес указывает на последнюю область сектора для записи настроек
    {
        EraseSector(ADDR_SECTOR_SETTINGS);                          // То стираем заполненный сектор настроек
        address = ADDR_SECTOR_SETTINGS;
    }

    CURRENT_PAGE = 0;
    WriteBufferBytes((uint)address, &set, sizeof(Settings));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FLASH_LoadSettins(void)
{
    if (READ_HALF_WORD(ADDR_SECTOR_SETTINGS) != 0xffff)     // Если настройки уже сохранялись
    {
        // Находим область сектора с сохранёнными настройками. Настройки записываются последовательно друг за другом по адресам, кратным SIZE_RECORD
        int address = ADDR_SECTOR_SETTINGS;
        do
        {
            address += SIZE_RECORD;
        } while(READ_HALF_WORD(address) != 0xffff && address < (int)(ADDR_SECTOR_SETTINGS + SIZE_SECTOR_SETTINGS));

        address -= SIZE_RECORD;

        // Читаем в Settings set количество байт, указанное в (int16)*address
        ReadBufferBytes((uint)address, &set, READ_HALF_WORD(address));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool EraseSector(uint startAddress)
{
    if (GetSector(startAddress) == MAX_UINT)
    {
        return false;
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

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteBufferBytes(uint address, void *buffer, int numBytes)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    for (int i = 0; i < numBytes; i++)
    {
        uint64_t data = ((uint8 *)buffer)[i];
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data);
        address++;
    }
    HAL_FLASH_Lock();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint GetSector(uint startAddress)
{
    typedef struct
    {
        uint number;
        uint startAddress;
    } StructSector;

    static const StructSector sectors[] =
    {
        {FLASH_SECTOR_11, ADDR_SECTOR_SETTINGS},
        {}
    };

    int i = 0;
    while (sectors[i].startAddress)
    {
        if (sectors[i].startAddress == startAddress)
        {
            return sectors[i].number;
        }
        i++;
    }

    return MAX_UINT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadBufferBytes(uint addrSrc, void *bufferDest, int size)
{
    uint8 *src = (uint8 *)addrSrc;
    uint8 *dest = (uint8 *)bufferDest;

    for (int i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
}
