#include "defines.h"
#include "FDrive_dl.h"
#include "Generator/Generator_d.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/String.h"
#include <usbh_diskio.h>


static USBH_HandleTypeDef handleUSBH;

USBH_HandleTypeDef *DLDrive::handle = &handleUSBH;

static FATFS FatFS;

static char USBDISKPath[4];
/// true, если флешка подключена
volatile static bool isConnected = false;

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


struct StructForReadDir
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
};


struct FileSystem
{
    
    /// Получить имя numFile-го файла из каталога fullPath
    static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut);
    /// Читает значения отсчётов сигнала из файла name
    static bool ReadFloats(float values[4096], char *name);
};




/// В эту функцию попадаем при каждом событии на OTG FS
static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id);
/// Получить имя numDir-го каталога из каталога fullPath
//static bool GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s);



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
            // здесь ничего
            break;
    }
}


void DLDrive::Init()
{
    HAL_HCD::Init();

    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK) //-V2001
    {
        USBH_StatusTypeDef res = USBH_Init(handle, USBH_UserProcess, 0);
        res = USBH_RegisterClass(handle, USBH_MSC_CLASS);
        res = USBH_Start(handle);
    }
}


void DLDrive::Update()
{
    USBH_Process(handle);

    if(state == State::NeedMount)
    {
        f_mount(&FatFS, USBDISKPath, 0);

        state = State::Connected;
    }
    else if(state == State::NeedUnmount)
    {
        f_mount(0, "", 0);

        state = State::Disconnected;
    }
}


void DLDrive::RequestFile(int num, char *fullPath)
{
    char name[255];

    FileSystem::GetNameFile(fullPath, num, name);
}


void DLDrive::RequestFileSize(int num, char *path)
{
    char name[255];

    if(FileSystem::GetNameFile(path, num, name))           // Получаем имя файла
    {
        String fullPath("%s\\%s", path, name);

        GetFileSize(fullPath.CString());
    }
}


void DLDrive::GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles)
{
    *numDirs = 0;
    *numFiles = 0;

    FILINFO fno;
    DIR dir;

    char nameDir[_MAX_LFN + 1];
    std::memcpy(nameDir, const_cast<char *>(fullPath), std::strlen(fullPath));
    nameDir[std::strlen(fullPath)] = '\0';

    if(f_opendir(&dir, nameDir) == FR_OK)
    {
        int numReadingElements = 0;
        bool alreadyNull = false;

        while(true) //-V2530
        {
            if(f_readdir(&dir, &fno) != FR_OK)
            {
                break;
            }
            if(fno.fname[0] == 0)
            {
                if(alreadyNull)
                {
                    break;
                }
                alreadyNull = true;
                continue;
            }
            numReadingElements++;
            if(fno.fname[0] != '.')
            {
                if(fno.fattrib & AM_DIR)
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


bool FileSystem::GetNameFile(const char *fullPath, int numFile, char *nameFileOut)
{
    StructForReadDir srd;

    std::memcpy(srd.nameDir, const_cast<char *>(fullPath), std::strlen(fullPath));
    srd.nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &srd.dir;
    FILINFO *pFNO = &srd.fno;
    if(f_opendir(pDir, srd.nameDir) == FR_OK)
    {
        int numFiles = 0;
        bool alreadyNull = false;
        while(true)
        {
            if(f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameFileOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if(pFNO->fname[0] == 0)
            {
                if(alreadyNull)
                {
                    *nameFileOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if(numFile == numFiles && (pFNO->fattrib & AM_DIR) == 0)
            {
                std::strcpy(nameFileOut, pFNO->fname);
                return true;
            }
            if((pFNO->fattrib & AM_DIR) == 0 && (pFNO->fname[0] != '.'))
            {
                numFiles++;
            }
        }
    }
    return false;
}


int DLDrive::GetFileSize(const char *fullPath)
{
    FIL fp;
    if(f_open(&fp, fullPath, FA_READ) == FR_OK)
    {
        uint size = f_size(&fp);
        f_close(&fp);
        return static_cast<int>(size);
    }

    return -1;
}


bool FileSystem::ReadFloats(float values[4096], char *name)
{
    bool result = false;

    FIL fp;
    FRESULT res = f_open(&fp, name, FA_READ);
    if(res == FR_OK)
    {
        char buffer[255];
        f_gets(buffer, 255, &fp);
        if(std::strcmp(buffer, "Rigol Technologies,Inc. Save analog waveform to text files.\r\n") == 0)
        {
            char *pointer = 0;
            int counter = 0;
            do
            {
                pointer = f_gets(buffer, 255, &fp);
                counter++;
            } while((std::strcmp(buffer, " 0 \r\n") != 0) && (pointer[0] == buffer[0]));

            if(pointer[0] == buffer[0])
            {
                for(int i = 0; i < 4096; i++)
                {
                    f_gets(buffer, 255, &fp);

                    char *ptr = std::strchr(buffer, ',');
                    if(ptr != 0)
                    {
                        *ptr = '.';
                    }

                    std::sscanf(buffer, "%e", &values[i]);

                    f_gets(buffer, 255, &fp);
                }

                result = true;
            }
        }
        f_close(&fp);
    }

    return result;
}



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
