#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "ffconf.h"
#include "ff.h"
#include "FatFS.h"
#include "FDrive/FDriveDevice.h"
#include "Generator/FPGA.h"
#include "Generator/GeneratorDevice.h"
#include "Interface/InterfaceDevice.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StructForReadDir
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Получает количество каталогов и файлов в данной директории
static void GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles);
/// Получить имя numFile-го файла из каталога fullPath
static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut);

static uint GetFileSize(char *fullPath);

static void ReadFloats(float values[4096], char *name);
/// Трансформировать точки в пригодный для записи в ПЛИС вид
static void TransformDataToCode(float d[4096], uint8 code[FPGA_NUM_POINTS * 2]);

static void Normalize(float d[4096]);

static void FindMinMax(float d[4096], float *_min, float *_max);

static float FindScale(float min, float max);

static void ToScale(float d[4096], float scale);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
        char name[255];

        int numFile = (int)msg->TakeByte();

        if (GetNameFile(msg->String(2), numFile, name))
        {
            Message *answer = new Message(Command::FDrive_RequestFile, (uint8)numFile, name);

            Interface::AddMessageForTransmit(answer);
        }
    }
    else if (com == Command::FDrive_RequestFileSize)
    {
        char name[255];
        int numFile = (int)msg->TakeByte();
        if (GetNameFile(msg->String(2), numFile, name))           // Получаем имя файла
        {
            String fullPath("%s\\%s", msg->String(2), name);

            uint size = GetFileSize(fullPath.CString());

            Message *answer = new Message(6, Command::FDrive_RequestFileSize, (uint8)numFile, size);

            Interface::AddMessageForTransmit(answer);
        }
    }
    else if (com == Command::FDrive_LoadToFPGA)
    {
        char fullName[255];
        Chan::E ch = (Chan::E)msg->TakeByte();
        int numFile = (int)msg->TakeByte();
        std::strcpy(fullName, msg->String(2));
        std::strcat(fullName, "\\");
        if (GetNameFile(msg->String(2), numFile, &fullName[std::strlen(fullName)]))
        {
            float values[4096];
            ReadFloats(values, &fullName[1]);
            uint8 data[FPGA_NUM_POINTS * 2];
            TransformDataToCode(values, data);

            // 1
            Generator::EnableChannel(ch, true);

            // 26
            std::memcpy(FPGA::DataDDS(ch), data, FPGA_NUM_POINTS * 2);
            Generator::SetFormWave(ch, Form::DDS);
        }
    }
    else if (com == Command::FDrive_GetPictureDDS)
    {
        /*
        int numFile = (int)msg->TakeByte();
        char fullName[255];
        std::strcpy(fullName, msg->String(2));
        std::strcpy(fullName, "\\");
        if(GetNameFile(msg->String()))

        Message *answer = new Message(2, Command::FDrive_GetPictureDDS, msg->TakeByte());
        Interface::AddMessageForTransmit(answer);
        */
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
static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut)
{
    StructForReadDir srd;

    std::memcpy(srd.nameDir, (void *)fullPath, std::strlen(fullPath));
    srd.nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &srd.dir;
    FILINFO *pFNO = &srd.fno;
    if (f_opendir(pDir, srd.nameDir) == FR_OK)
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
static void ReadFloats(float values[4096], char *name)
{
    FIL fp;
    FRESULT result = f_open(&fp, name, FA_READ);
    if (result == FR_OK)
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

                    char *ptr = std::strchr(buffer, ',');
                    if (ptr != 0)
                    {
                        *ptr = '.';
                    }

                    std::sscanf(buffer, "%e", &values[i]);

                    f_gets(buffer, 255, &fp);
                }
            }
        }
        f_close(&fp);
    }
    else
    {
        LOG_ERROR("Произошла ошибка при открытии файла %s", name);
        LOG_ERROR("%s", FatFS::ErrorString(result).CString());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TransformDataToCode(float d[4096], uint8 code[FPGA_NUM_POINTS * 2])
{
    Normalize(d);

    int max = 0x1fff;

    for (int i = 0; i < 4096; i++)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Normalize(float d[4096])
{
    float min = 0.0f;
    float max = 0.0f;

    FindMinMax(d, &min, &max);

    float scale = FindScale(min, max);

    ToScale(d, scale);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FindMinMax(float d[4096], float *_min, float *_max)
{
    float min = 0.0f;
    float max = 0.0f;

    for (int i = 0; i < 4096; i++)
    {
        if (d[i] < min)
        {
            min = d[i];
        }
        if (d[i] > max)
        {
            max = d[i];
        }
    }

    *_min = min;
    *_max = max;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float FindScale(float min, float max)
{
    max = std::fabsf(max);

    if (std::fabsf(min) > max)
    {
        max = std::fabsf(min);
    }

    return 1.0f / max;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ToScale(float d[4096], float scale)
{
    for (int i = 0; i < 4096; i++)
    {
        d[i] *= scale;
    }
}
