#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "structs.h"
#include "Log.h"
#include "FDriveDevice.h"
#include "Interface/InterfaceDevice.h"
#include "Hardware/CPU.h"
#include "Utils/Console.h"
#include "usbh_diskio.h"
#include "usbh_conf.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBH_HandleTypeDef FDrive::hUSB_Host;

static FATFS FatFS;

static char USBDISKPath[4];
/// true, если флешка подключена
volatile static bool isConnected = false;
/// Количество байт для передачи в Interface
static uint numBytesForSend = 0;
/// Начало буфера данных для передачи в Interface
static uint8 *bufferForSend = 0;
/// Путь к каталогу, количество файлов и каталогов в котором нужно узнать
//static char path[256];
/// Номер запрашиваемого имени - каталога или файла
//static uint numItem = 0;

struct StructForReadDir
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
};


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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В эту функцию попадаем при каждом событии на OTG FS
static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id);
/// Получает количество каталогов и файлов в данной директории
static void GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles);
/// Подготовить буфер для даныых
static void PrepareBufferForData(uint size, uint8 command);
/// Получить имя numDir-го каталога из каталога fullPath
//static bool GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s);
/// Получить имя numFile-го файла из каталога fullPath
//static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Update()
{
    USBH_Process(&hUSB_Host);

    if(state == State::NeedMount)
    {
        if(f_mount(&FatFS, USBDISKPath, 0) == FR_OK)
        {
            PrepareBufferForData(1 + 1, Command::FDrive_Mount);

            bufferForSend[1] = 1;
        }

        state = State::Connected;

    }
    else if(state == State::NeedUnmount)
    {
        f_mount(0, "", 0);

        PrepareBufferForData(1 + 1, Command::FDrive_Mount);

        bufferForSend[1] = 0;

        state = State::Disconnected;
    }
    else if(command == Command::FDrive_NumDirsAndFiles)
    {
        /*
        PrepareBufferForData(1 + 4 + 4, Command::FDrive_NumDirsAndFiles);

        BitSet32 numDirs;
        BitSet32 numFiles;

        GetNumDirsAndFiles(path, &numDirs.word, &numFiles.word);

        numDirs.WriteToBuffer(bufferForSend + 1);
        numFiles.WriteToBuffer(bufferForSend + 5);

        command = Command::Number;
        */
    }
    else if(command == Command::FDrive_RequestDir)
    {
        /*
        char buffer[256];
        StructForReadDir str;
        GetNameDir(path, (int)numItem, buffer, &str);

        PrepareBufferForData(1 + 4 + strlen(buffer) + 1, Command::FDrive_RequestDir);

        BitSet32 num;
        num.word = numItem;
        num.WriteToBuffer(bufferForSend + 1);
        strcpy((char *)bufferForSend + 5, buffer);

        command = Command::Number;
        */
    }
    else if(command == Command::FDrive_RequestFile)
    {
        /*
        char buffer[256];
        StructForReadDir str;
        GetNameFile(path, (int)numItem, buffer, &str);

        PrepareBufferForData(1 + 4 + strlen(buffer) + 1, Command::FDrive_RequestFile);

        BitSet32 num;
        num.word = numItem;
        num.WriteToBuffer(bufferForSend + 1);
        strcpy((char *)bufferForSend + 5, buffer);

        command = Command::Number;
        */
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareBufferForData(uint size, uint8 com)
{
    numBytesForSend = size;
    bufferForSend = (uint8 *)std::malloc(numBytesForSend);
    *bufferForSend = com;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FDrive::NumBytesForSend()
{
    return numBytesForSend;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *FDrive::GetDataForSend(uint8 *buffer)
{
    memcpy(buffer, bufferForSend, numBytesForSend);
    std::free(bufferForSend);
    numBytesForSend = 0;
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::HandlerInterface()
{
    command = *Interface::recv;

    if(command == Command::FDrive_NumDirsAndFiles)
    {
        uint numDirs = 0;
        uint numFiles = 0;
        GetNumDirsAndFiles((const char *)Interface::recv + 1, &numDirs, &numFiles);
        uint8 *buffer = (uint8 *)std::malloc(1 + 4 + 4);

        buffer[0] = Command::FDrive_NumDirsAndFiles;
        memcpy(buffer + 1, &numDirs, 4);
        memcpy(buffer + 5, &numFiles, 4);
        Interface::Send(buffer, 9);

        std::free(buffer);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles)
{
    FILINFO fno;
    DIR dir;

    *numDirs = 0;
    *numFiles = 0;

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
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
static bool GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s)
{
    memcpy(s->nameDir, (void *)fullPath, strlen(fullPath));
    s->nameDir[strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numDirs = 0;
        FILINFO *pFNO = &s->fno;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameDirOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numDir == numDirs && (pFNO->fattrib & AM_DIR))
            {
                strcpy(nameDirOut, pFNO->fname);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) && (pFNO->fname[0] != '.'))
            {
                numDirs++;
            }
        }
    }
    return false;
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s)
{
    memcpy(s->nameDir, (void *)fullPath, strlen(fullPath));
    s->nameDir[strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numFiles = 0;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameFileOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameFileOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numFile == numFiles && (pFNO->fattrib & AM_DIR) == 0)
            {
                strcpy(nameFileOut, pFNO->fname);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) == 0 && (pFNO->fname[0] != '.'))
            {
                numFiles++;
            }
        }
    }
    return false;
}
*/
