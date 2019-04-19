#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "libs/FatFS/ffconf.h"
#include "libs/FatFS/FatFS.h"
#include "FDrive/FDriveDevice.h"
#include "Generator/FPGA.h"
#include "Generator/GeneratorDevice.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include <ff.h>
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

static bool ReadFloats(float values[4096], char *name);
/// Трансформировать точки в пригодный для записи в ПЛИС вид
static void TransformDataToCode(float d[4096], uint8 code[FPGA::NUM_POINTS * 2]);

static void Normalize(float d[4096]);

static void FindMinMax(float d[4096], float *_min, float *_max);

static float FindScale(float min, float max);

static void ToScale(float d[4096], float scale);
/// Заполнить массив picture данными для отрисовки сигнала на экране
static void FillPicture(uint8 *picture, uint size, float values[4096]);


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

        MessageNumDirsAndFiles(numDirs, numFiles).Transmit();
    }
    else if (com == Command::FDrive_RequestFile)
    {
        char name[255];

        int numFile = (int)msg->TakeByte();

        if (GetNameFile(msg->String(2), numFile, name))
        {
            Message(Command::FDrive_RequestFile, (uint8)numFile, name).Transmit();
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

            Message(6, Command::FDrive_RequestFileSize, (uint8)numFile, size).Transmit();
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
            Buffer buffer(8 * 1024 * sizeof(float));
            ReadFloats(buffer.DataFloat(), &fullName[1]);
            TransformDataToCode(buffer.DataFloat(), FPGA::DataFlash(ch));
        }
    }
    else if (com == Command::FDrive_GetPictureDDS)
    {
        const uint SIZE = 240;
        uint8 data[SIZE];
        std::memset(data, 0, SIZE);

        int numFile = (int)msg->TakeByte();

        char fullName[255];
        std::strcpy(fullName, msg->String(2));
        std::strcpy(fullName, "\\");
        
        if (GetNameFile(msg->String(2), numFile, &fullName[std::strlen(fullName)]))
        {
            float values[4096];
            if (ReadFloats(values, &fullName[1]))
            {
                FillPicture(data, SIZE, values);
            }
        }

        Message answer(2 + SIZE, Command::FDrive_GetPictureDDS, (uint8)numFile);
        answer.PutData(data, SIZE);
        answer.Transmit();
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
    else
    {
        // здесь ничего
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
static bool ReadFloats(float values[4096], char *name)
{
    bool result = false;

    FIL fp;
    FRESULT res = f_open(&fp, name, FA_READ);
    if (res == FR_OK)
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

                result = true;
            }
        }
        f_close(&fp);
    }
    else
    {
        LOG_ERROR("Произошла ошибка при открытии файла %s", name);
        LOG_ERROR("%s", FatFS::ErrorString(res).CString());
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TransformDataToCode(float d[4096], uint8 code[FPGA::NUM_POINTS * 2])
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
        code[i * 2 + FPGA::NUM_POINTS] = code[i * 2 + FPGA::NUM_POINTS + 1] = (uint8)(c >> 8);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FillPicture(uint8 *picture, uint size, float values[4096])
{
    Normalize(values);

    uint8 aveValue = 127;

    float step = 4096.0f / size;

    for (uint i = 0; i < size; i++)
    {
        float val = values[(int)(i * step)];

        picture[i] = (uint8)(aveValue + val * 125);
    }
}
