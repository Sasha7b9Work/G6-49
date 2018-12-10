#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "FDriveDevice.h"
#include "Interface/InterfaceDevice.h"
#include "Generator/FPGA.h"
#include "Generator/GeneratorDevice.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include "usbh_diskio.h"
#include "usbh_conf.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBH_HandleTypeDef FDrive::hUSB_Host;

static FATFS FatFS;

static char USBDISKPath[4];
/// true, если флешка подключена
volatile static bool isConnected = false;
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
/// Получить имя numDir-го каталога из каталога fullPath
//static bool GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s);
/// Получить имя numFile-го файла из каталога fullPath
//static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s);
/// Трансформировать точки в пригодный для записи в ПЛИС вид
static void TransformDataToCode(float d[4095], uint8 code[FPGA_NUM_POINTS * 2]);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch(id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            //LOG_WRITE("HOST_USER_SELECT_CONFIGURATION");
            break;

        case HOST_USER_CLASS_ACTIVE:
            //LOG_WRITE("HOST_USER_CLASS_ACTIVE");
            state = State::NeedMount;
            break;

        case HOST_USER_CLASS_SELECTED:
            //LOG_WRITE("HOST_USER_CLASS_SELECTED");
            break;

        case HOST_USER_CONNECTION:
            //LOG_WRITE("HOST_USER_CONNECTION");
            break;

        case HOST_USER_DISCONNECTION:
            //LOG_WRITE("HOST_USER_DISCONNECTION");
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
        FRESULT result = f_mount(&FatFS, USBDISKPath, 0);

        Message *message = new Message(2, Command::FDrive_Mount, (result == FR_OK) ? (uint8)1 : (uint8)2);
        Interface::AddMessageForTransmit(message);

        state = State::Connected;

    }
    else if(state == State::NeedUnmount)
    {
        f_mount(0, "", 0);

        Message *message = new Message(2, Command::FDrive_Mount, (uint8)0);
        Interface::AddMessageForTransmit(message);

        state = State::Disconnected;
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
static uint GetFileSize(char *fullPath)
{
    FIL fp;
    if (f_open(&fp, fullPath, FA_READ) == FR_OK)
    {
        uint size = f_size(&fp);
        f_close(&fp);
        return size;
    }

    return (uint)-1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s)
{
    std::memcpy(s->nameDir, (void *)fullPath, std::strlen(fullPath));
    s->nameDir[std::strlen(fullPath)] = '\0';

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
                std::strcpy(nameFileOut, pFNO->fname);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles)
{
    FILINFO fno;
    DIR dir;

    *numDirs = 0;
    *numFiles = 0;

    char nameDir[_MAX_LFN + 1];
    std::memcpy(nameDir, (void *)fullPath, std::strlen(fullPath));
    nameDir[std::strlen(fullPath)] = '\0';

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
static void ReadFloats(float values[4096], char *name)
{
    FIL fp;
    if (f_open(&fp, name, FA_READ) == FR_OK)
    {
        char buffer[255];
        f_gets(buffer, 255, &fp);
        if (std::strcmp(buffer, "Rigol Technologies,Inc. Save analog waveform to text files.\r\n") == 0)
        {
            char *pointer = 0;
            int counter = 0;
            do
            {
                pointer = f_gets(buffer, 255, &fp);
                counter++;
            } while ((std::strcmp(buffer, " 0 \r\n") != 0) && (pointer[0] == buffer[0]));

            if (pointer[0] == buffer[0])
            {
                for (int i = 0; i < 4096; i++)
                {
                    f_gets(buffer, 255, &fp);
#ifndef WIN32
                    char *ptr = std::strchr(buffer, ',');
                    if (ptr != 0)
                    {
                        *ptr = '.';
                    }
                    std::sscanf(buffer, "%e", &values[i]);
#endif

                    f_gets(buffer, 255, &fp);
                }
            }
        }
        f_close(&fp);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Handler::Processing(Message *msg)
{
    msg->ResetPointer();

    uint8 com = msg->TakeByte();

    if (com == Command::FDrive_NumDirsAndFiles)
    {
        uint numDirs = 0;
        uint numFiles = 0;

        GetNumDirsAndFiles(msg->String(1), &numDirs, &numFiles);

        Message *answer = new Message(9, Command::FDrive_NumDirsAndFiles, numDirs, numFiles);

        Interface::AddMessageForTransmit(answer);
    }
    else if (com == Command::FDrive_RequestFile)
    {
        StructForReadDir srd;

        char name[255];

        int numFile = (int)msg->TakeByte();

        if (GetNameFile(msg->String(2), numFile, name, &srd))
        {
            Message *answer = new Message(Command::FDrive_RequestFile, (uint8)numFile, name);

            Interface::AddMessageForTransmit(answer);
        }
    }
    else if (com == Command::FDrive_RequestFileSize)
    {
        StructForReadDir srd;
        char name[255];
        int numFile = (int)msg->TakeByte();
        if (GetNameFile(msg->String(2), numFile, name, &srd))           // Получаем имя файла
        {
            String fullPath("%s\\%s", msg->String(2), name);

            uint size = GetFileSize(fullPath.CString());

            Message *answer = new Message(6, Command::FDrive_RequestFileSize, (uint8)numFile, size);

            Interface::AddMessageForTransmit(answer);
        }
    }
    else if (com == Command::FDrive_LoadToFPGA)
    {
        StructForReadDir srd;
        char fullName[255];
        Chan::E ch = (Chan::E)msg->TakeByte();
        int numFile = (int)msg->TakeByte();
        std::strcpy(fullName, msg->String(2));
        std::strcat(fullName, "\\");
        if (GetNameFile(msg->String(2), numFile, &fullName[std::strlen(fullName)], &srd))
        {
            float values[4096];
            ReadFloats(values, fullName);
            uint8 data[8129 * 2];
            TransformDataToCode(values, data);
            std::memcpy(FPGA::DataDDS(ch), data, 8192 * 2);
            Generator::SetFormWave(ch, Form::DDS);
        }
    }
    else if (com == Command::FDrive_RequestFileString)
    {
        LOG_WRITE("Запрос на строку %d", msg->TakeWord());

        /*
        uint numString = msg->TakeWord();

        char string[256];                                   // Здесь будет ответ

        char *out = 0;

        FIL fp;
        if(f_open(&fp, msg->String(2), FA_READ) == FR_OK)
        {
            for (uint i = 0; i < numString; i++)
            {
                out = f_gets(string, 256, &fp);
            }
            f_close(&fp);
        }

        if (out != &string[0])
        {
            string[0] = 0;
        }

        Message *answer = new Message(Command::FDrive_RequestFileString, numString, string);

        Interface::AddMessageForTransmit(answer);
        */
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TransformDataToCode(float d[4095], uint8 code[FPGA_NUM_POINTS * 2])
{
    int max = 0x1fff;

    for (int i = 0; i < 4095; i++)
    {
        uint16 c = (uint16)(std::fabsf(d[i]) * max);

        if (Sign(d[i]) == -1)
        {
            SetBit(c, 13);
        }

        code[i * 2] = code[i * 2 + 1] = (uint8)c;
        code[i * 2 + FPGA_NUM_POINTS] = code[i * 2 + FPGA_NUM_POINTS + 1] = (uint8)(c >> 8);
    }
}
