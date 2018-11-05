#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Log.h"
#include "FDriveDevice.h"
#include "Interface/InterfaceDevice.h"
#include "Hardware/CPU/CPU.h"
#include "Utils/Console.h"
#include "usbh_diskio.h"
#include "usbh_conf.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBH_HandleTypeDef FDrive::hUSB_Host;

static FATFS FatFS;

static char USBDISKPath[4];
/// true, если флешка подключена
volatile static bool isConnected = false;
/// Количество байт для передачи в Interface
static uint16 numBytesForSend = 0;
/// Начало буфера данных для передачи в Interface
static uint8 *bufferForSend = 0;
/// Если true, то устройство занято и обмен с интерфейсом запрещён
static bool isBusy = false;
/// Путь к каталогу, количество файлов и каталогов в котором нужно узнать
static char path[256];


struct State
{
    enum E
    {
        Disconnected,   ///< Начальное значение после старта
        NeedMount,      ///< Обнаружена подключенная флешка, требуется монтирование
        Connected,      ///< Флешка подсоединена и примонтирована
        NeedUnmount     ///< Требуется отмонтировать
    } value;
};

static State::E state = State::Disconnected;
/// Здесь хранится обрабатываемая команда
static Command command = Command::Number;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В эту функцию попадаем при каждом событии на OTG FS
static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id);
/// Получает количество каталогов и файлов в данной директории
static void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch(id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_CLASS_ACTIVE:
            state = State::NeedMount;
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            break;

        case HOST_USER_DISCONNECTION:
            state = State::NeedUnmount;
            break;

        default:
            break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Init()
{
    CPU::HCD::Init();

    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK)
    {
        USBH_StatusTypeDef res = USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
        res = USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
        res = USBH_Start(&hUSB_Host);
    }
    else
    {
        LOG_ERROR("Can not %s", __FUNCTION__);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Update()
{
    if(numBytesForSend)
    {
        return;
    }

    isBusy = true;

    USBH_Process(&hUSB_Host);

    if(state == State::NeedMount)
    {

        if(f_mount(&FatFS, USBDISKPath, 0) == FR_OK)
        {
            numBytesForSend = 1 + 1;
            bufferForSend = (uint8 *)malloc(numBytesForSend);
            bufferForSend[0] = Command::FDrive_Mount;
            bufferForSend[1] = 1;
        }

        state = State::Connected;

    }
    else if(state == State::NeedUnmount)
    {
        f_mount(0, "", 0);

        numBytesForSend = 1 + 1;
        bufferForSend = (uint8 *)malloc(numBytesForSend);
        bufferForSend[0] = Command::FDrive_Mount;
        bufferForSend[1] = 0;

        state = State::Disconnected;
    }
    else if(command == Command::FDrive_NumDirsAndFiles)
    {
        int numDirs;
        int numFiles;
        GetNumDirsAndFiles(path, &numDirs, &numFiles);
        command = Command::Number;
    }

    isBusy = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 FDrive::NumBytesForSend()
{
    if(isBusy)
    {
        return 0;
    }
    return numBytesForSend;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *FDrive::GetDataForSend(uint8 *buffer)
{
    memcpy(buffer, bufferForSend, numBytesForSend);
    free(bufferForSend);
    numBytesForSend = 0;
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::HandlerInterface()
{
    isBusy = true;

    command = *Interface::recv;

    if(command == Command::FDrive_NumDirsAndFiles)
    {
        uint8 *src = Interface::recv + 1;
        char *dest = &path[0];
        while(*src)
        {
            *dest++ = (char)*src++;
        }
        *dest = '\0';
    }

    isBusy = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles)
{
    FILINFO fno;
    DIR dir;

    *numDirs = 0;
    *numFiles = 0;

    Console::AddString(fullPath);


    char nameDir[_MAX_LFN + 1];
    memcpy(nameDir, (void *)fullPath, strlen(fullPath));
    nameDir[strlen(fullPath)] = '\0';

    if (f_opendir(&dir, nameDir) == FR_OK)
    {
        int numReadingElements = 0;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(&dir, &fno) != FR_OK)
            {
                break;
            }
            if (fno.fname[0] == 0)
            {
                if (alreadyNull)
                {
                    break;
                }
                alreadyNull = true;
                continue;
            }
            numReadingElements++;
            if (fno.fname[0] != '.')
            {
                if (fno.fattrib & AM_DIR)
                {
                    (*numDirs)++;
                }
                else
                {
                    (*numFiles)++;
                }
            }
        }
        f_closedir(&dir);
    }

    Console::AddInt(*numDirs);
    Console::AddInt(*numFiles);
}
