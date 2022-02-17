#include "defines.h"
#include "ffconf.h"
#include "Hardware/FatFS/FatFS.h"
#include "FDrive/FDrive_d.h"
#include "Generator/FPGA.h"
#include "Generator/Generator_d.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include <ff.h>
#include <cstdio>
#include <cmath>


struct StructForReadDir
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
};


static SimpleMessage *msg = nullptr;

static void GetNumDirsAndFiles();
static void RequestFile();
static void RequestFileSize();

// Загрузить сигнал с флешки
static void LoadDDSfromFile();
static void GetPictureDDS();

static void CreateFile();
static void WriteToFile();
static void CloseFile();

// Трансформировать точки в пригодный для записи в ПЛИС вид
static void TransformDataToCode(float dataIn[FPGA::NUM_POINTS], uint8 codeOut[FPGA::NUM_POINTS * 2]);

// Заполнить массив picture данными для отрисовки сигнала на экране
static void FillPicture(uint8 *picture, uint size, float values[FPGA::NUM_POINTS]);
static void Normalize(float d[FPGA::NUM_POINTS]);
static void FindMinMax(const float d[FPGA::NUM_POINTS], float *_min, float *_max);
static float FindScale(float min, float max);
static void ToScale(float d[FPGA::NUM_POINTS], float scale);


static void E()
{

}


struct FileSystem
{
    static uint GetFileSize(const char *fullPath);
    // Получает количество каталогов и файлов в данной директории
    static void GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles);
    // Получить имя numFile-го файла из каталога fullPath
    static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut);
    // Читает значения отсчётов сигнала из файла name
    static bool ReadFloats(float values[FPGA::NUM_POINTS], const char *name);
};


void DDrive::Handler::Processing(SimpleMessage *message)
{
    msg = message;

    msg->ResetPointer();

    uint8 com = msg->TakeUINT8();

    pFuncVV func = E;

    switch(com)
    {
    case Command::FDrive_NumDirsAndFiles:               func = GetNumDirsAndFiles;  break;
    case Command::FDrive_RequestFile:                   func = RequestFile;         break;
    case Command::FDrive_RequestFileSize:               func = RequestFileSize;     break;
    case Command::FDrive_LoadDDSfromFile:               func = LoadDDSfromFile;     break;
    case Command::FDrive_RequestPictureDDSfromFile:     func = GetPictureDDS;       break;
    case Command::FDrive_CreateFile:                    func = CreateFile;          break;
    case Command::FDrive_WriteToFile:                   func = WriteToFile;         break;
    case Command::FDrive_CloseFile:                     func = CloseFile;           break;
    }

    func();
}


static void GetNumDirsAndFiles()
{
    uint numDirs = 0;
    uint numFiles = 0;

    FileSystem::GetNumDirsAndFiles(msg->String(1), &numDirs, &numFiles);

    Message::FDrive::NumDirsAndFiles(numDirs, numFiles).Transmit();
}


static void RequestFile()
{
    char name[255];

    int numFile = static_cast<int>(msg->TakeUINT8());

    if (FileSystem::GetNameFile(msg->String(2), numFile, name))
    {
        Message::FDrive::FileName(static_cast<uint8>(numFile), name).Transmit();
    }
}


static void RequestFileSize()
{
    char name[255];
    int numFile = static_cast<int>(msg->TakeUINT8());
    if (FileSystem::GetNameFile(msg->String(2), numFile, name))           // Получаем имя файла
    {
        String fullPath("%s\\%s", msg->String(2), name);

        uint size = FileSystem::GetFileSize(fullPath.CString());

        Message::FDrive::FileSize(static_cast<uint8>(numFile), size).Transmit();
    }
}


static void LoadDDSfromFile()
{
    char fullName[255];
    Chan ch(msg->TakeUINT8());
    int numFile = static_cast<int>(msg->TakeUINT8());
    std::strcpy(fullName, msg->String(2));
    std::strcat(fullName, "\\");

    if (FileSystem::GetNameFile(msg->String(2), numFile, &fullName[std::strlen(fullName)]))
    {
        Buffer buffer(DGenerator::DDS_NUM_POINTS * sizeof(float));
        FileSystem::ReadFloats(buffer.DataFloat(), &fullName[1]);
        uint8 code[DGenerator::DDS_NUM_POINTS * 2];
        TransformDataToCode(buffer.DataFloat(), code);
        FPGA::SaveExtSignal(ch, code);

        Message::FDrive::LoadDDSfromFile(ch, static_cast<uint8>(numFile), 0).Transmit();     // Посылаем признак того, что сохранение завершено
    }
}


static void GetPictureDDS()
{
    const uint SIZE = 240;
    uint8 data[SIZE];
    std::memset(data, 0, SIZE);

    int numFile = static_cast<int>(msg->TakeUINT8());

    char fullName[255];
    std::strcpy(fullName, msg->String(2));
    std::strcpy(fullName, "\\");

    if (FileSystem::GetNameFile(msg->String(2), numFile, &fullName[std::strlen(fullName)]))
    {
        float values[FPGA::NUM_POINTS];
        if (FileSystem::ReadFloats(values, &fullName[1]))
        {
            FillPicture(data, SIZE, values);
        }
    }

    Message::FDrive::RequestPictureDDSfromFile(static_cast<uint8>(numFile), data).Transmit();
}


// Возвращает true, если расширение соответствует расширению сигнала
static bool ExtensionIsSignal(const TCHAR *name, const char *ext = "sig")
{
    uint lenght = std::strlen(name);

    return (name[lenght - 1] == ext[2]) &&
           (name[lenght - 2] == ext[1]) &&
           (name[lenght - 3] == ext[0]);
}


void FileSystem::GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles)
{
    FILINFO fno;
    DIR dir;

    *numDirs = 0;
    *numFiles = 0;

    char nameDir[_MAX_LFN + 1];
    std::memcpy(nameDir, const_cast<char *>(fullPath), std::strlen(fullPath)); //-V2567
    nameDir[std::strlen(fullPath)] = '\0';

    if (f_opendir(&dir, nameDir) == FR_OK)
    {
        int numReadingElements = 0;
        bool alreadyNull = false;

        while (true) //-V2530
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
                else if(ExtensionIsSignal(fno.fname))
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

    std::memcpy(srd.nameDir, const_cast<char *>(fullPath), std::strlen(fullPath)); //-V2567
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
            if (numFile == numFiles && (pFNO->fattrib & AM_DIR) == 0 && ExtensionIsSignal(pFNO->fname))
            {
                std::strcpy(nameFileOut, pFNO->fname);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) == 0 && (pFNO->fname[0] != '.'))
            {
                if (ExtensionIsSignal(pFNO->fname))
                {
                    numFiles++;
                }
            }
        }
    }
    return false;
}


uint FileSystem::GetFileSize(const char *fullPath)
{
    FIL fp;
    if (f_open(&fp, fullPath, FA_READ) == FR_OK)
    {
        uint size = f_size(&fp);
        f_close(&fp);
        return size;
    }

    return static_cast<uint>(-1);
}


// Конвертировать считанное из файла значение в float [-1.0 ... 1.0]
static float ConvertToFloat(int value)
{
    return -1.0F + static_cast<float>(value) / 4095.0F * 2.0F;
}


// Интерполировать значения в массиве values между индексами iFilledLast и iReaded
static void Interpolate(float values[FPGA::NUM_POINTS], int iFilledLast, int iReaded)
{
    int numSteps = iReaded - iFilledLast;
    float stepY = (values[iReaded] - values[iFilledLast]) / static_cast<float>(numSteps);

    for (int i = 1; i < numSteps; i++)
    {
        values[i + iFilledLast] = values[iFilledLast] + static_cast<float>(i) * stepY;
    }
}


bool FileSystem::ReadFloats(float values[FPGA::NUM_POINTS], const char *name)
{
    bool result = false;

    FIL fp;
    FRESULT res = f_open(&fp, name, FA_READ);
    if (res == FR_OK)
    {
        char buffer[255];
        f_gets(buffer, 255, &fp);

        if (std::strcmp(buffer, "Data file G6-49\r\n") == 0)
        {
            f_gets(buffer, 255, &fp);

            if (std::strcmp(buffer, "points\r\n") == 0)
            {
                int indexFilled = -1;                        // Индекс последней рассчитанной и занесённой точки

                while (indexFilled < (FPGA::NUM_POINTS - 1))
                {
                    f_gets(buffer, 255, &fp);

                    int indexReaded = 0;                    // Индекс считанной точки
                    int value = 0;

                    std::sscanf(buffer, "%d %d", &indexReaded, &value);

                    if (indexReaded == 0)
                    {
                        indexFilled = 0;
                        values[indexFilled] = ConvertToFloat(value);
                    }
                    else if(indexReaded == indexFilled + 1)
                    {
                        indexFilled = indexReaded;
                        values[indexFilled] = ConvertToFloat(value);
                    }
                    else
                    {
                        values[indexReaded] = ConvertToFloat(value);
                        Interpolate(values, indexFilled, indexReaded);
                        indexFilled = indexReaded;
                    }
                }
            }

            result = true;
        }


        f_close(&fp);
    }

    return result;
}


static void TransformDataToCode(float dataIn[FPGA::NUM_POINTS], uint8 codeOut[FPGA::NUM_POINTS * 2])
{
    Normalize(dataIn);

    float max = static_cast<float>(0x1fff);

    for (int i = 0; i < FPGA::NUM_POINTS; i++)
    {
        uint16 c = static_cast<uint16>(std::fabsf(dataIn[i]) * max);

        if (dataIn[i] > 0.0F)
        {
            SetBit(c, 13);
        }

        codeOut[i]                    = static_cast<uint8>(c);
        codeOut[i + FPGA::NUM_POINTS] = static_cast<uint8>(c >> 8);
    }
}


static void Normalize(float d[FPGA::NUM_POINTS])
{
    float min = 0.0F;
    float max = 0.0F;

    FindMinMax(d, &min, &max);

    float scale = FindScale(min, max);

    ToScale(d, scale);
}


static void FindMinMax(const float d[FPGA::NUM_POINTS], float *_min, float *_max)
{
    float min = 0.0F;
    float max = 0.0F;

    for (int i = 0; i < FPGA::NUM_POINTS; i++)
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


static float FindScale(float min, float max)
{
    max = std::fabsf(max);

    if (std::fabsf(min) > max)
    {
        max = std::fabsf(min);
    }

    return 1.0F / max;
}


static void ToScale(float d[FPGA::NUM_POINTS], float scale)
{
    for (int i = 0; i < FPGA::NUM_POINTS; i++)
    {
        d[i] *= scale;
    }
}


static void FillPicture(uint8 *picture, uint size, float values[FPGA::NUM_POINTS])
{
    Normalize(values);

    float aveValue = 127.0F;

    float step = static_cast<float>(FPGA::NUM_POINTS) / static_cast<float>(size);

    for (uint i = 0; i < size; i++)
    {
        float val = values[static_cast<int>(static_cast<float>(i) * step)];

        picture[i] = static_cast<uint8>(aveValue + val * 125.0F);
    }
}


static FIL fileObj;


static void CreateFile()
{
    char fullName[256];

    std::strcpy(fullName, msg->String(1));

    f_open(&fileObj, fullName, FA_CREATE_ALWAYS | FA_WRITE);
}


static void WriteToFile()
{
    uint wr = 0;
    uint size = static_cast<uint>(msg->TakeINT());
    f_write(&fileObj, msg->TakeData(5), size, &wr);
}


static void CloseFile()
{
    f_close(&fileObj);
}
