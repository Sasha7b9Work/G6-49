#include "defines.h"
#include "libs/FatFS/ffconf.h"
#include "libs/FatFS/FatFS.h"
#include "FDrive/FDrive_dl.h"
#include "Generator/Generator_d.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include <ff.h>
#include <cstdio>
#include <cmath>


SimpleMessage *DDrive::Handler::msg = nullptr;


static void E()
{

}


struct StructForReadDir
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
};


struct FileSystem
{
    static uint GetFileSize(const char *fullPath);
    /// Получает количество каталогов и файлов в данной директории
    static void GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles);
    /// Получить имя numFile-го файла из каталога fullPath
    static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut);
    /// Читает значения отсчётов сигнала из файла name
    static bool ReadFloats(float values[4096], char *name);
};


void DDrive::Handler::Processing(SimpleMessage *message)
{
    msg = message;

    msg->ResetPointer();

    uint8 com = msg->TakeByte();

    static const pFuncVV funcs[Command::Count] =
    {
        /* RequestData                  */ E,
        /* EnableChannel                */ E,
        /* SetFormWave                  */ E,
        /* SetFrequency                 */ E,
        /* SetAmplitude                 */ E,
        /* SetOffset                    */ E,
        /* SetDuration                  */ E,
        /* SetDutyRatio                 */ E,
        /* SetPhase                     */ E,
        /* RunReset                     */ E,
        /* ModeDebug                    */ E,
        /* SetDelay                     */ E,
        /* WriteRegister                */ E,
        /* SetDurationRise              */ E,
        /* SetDurationFall              */ E,
        /* SetDurationStady             */ E,
        /* SetDutyFactor                */ E,
        /* SetManipulation              */ E,
        /* SetManipulationDuration      */ E,
        /* SetManipulationPeriod        */ E,
        /* SetPacketPeriod              */ E,
        /* SetPacketNumber              */ E,
        /* SetStartMode                 */ E,
        /* SetPeriod                    */ E,
        /* SetPolarity                  */ E,
        /* SetManipulationMode          */ E,
        /* LoadFromDDS                  */ E,
        /* FreqMeasure                  */ E,
        /* Log                          */ E,
        /* FDrive_NumDirsAndFiles       */ E,
        /* FDrive_Mount                 */ E,
        /* FDrive_RequestDir            */ E,
        /* FDrive_RequestFile           */ E,
        /* Test                         */ E,
        /* SetKoeffCalibration          */ E,
        /* GetKoeffCalibration          */ E,
        /* FDrive_RequestFileSize       */ E,
        /* FDrive_RequestFileString     */ E,
        /* FDrive_LoadFromExtStorage    */ E,
        /* FDrive_GetPictureDDS         */ E,
        /* SCPI_RecvData                */ E,
        /* PortCPU                      */ E,
        /* CalibrationLoad              */ E,
        /* CalibrationSet               */ E,
        /* StartApplication             */ E,
        /* RequestUpdate                */ E
    };

    funcs[com]();
}


void DDrive::Handler::GetNumDirsAndFiles()
{
    uint numDirs = 0;
    uint numFiles = 0;

    FileSystem::GetNumDirsAndFiles(msg->String(1), &numDirs, &numFiles);

    Message::FDrive::NumDirsAndFiles(numDirs, numFiles).Transmit();
}


void DDrive::Handler::RequestFile()
{
    char name[255];

    int numFile = static_cast<int>(msg->TakeByte());

    if (FileSystem::GetNameFile(msg->String(2), numFile, name))
    {
        Message::FDrive::FileName(static_cast<uint8>(numFile), name).Transmit();
    }
}


void DDrive::Handler::RequestFileSize()
{
    char name[255];
    int numFile = static_cast<int>(msg->TakeByte());
    if (FileSystem::GetNameFile(msg->String(2), numFile, name))           // Получаем имя файла
    {
        String fullPath("%s\\%s", msg->String(2), name);

        uint size = FileSystem::GetFileSize(fullPath.CString());

        Message::FDrive::FileSize(static_cast<uint8>(numFile), size).Transmit();
    }
}


void DDrive::Handler::GetPictureDDS()
{
    const uint SIZE = 240;
    uint8 data[SIZE];
    std::memset(data, 0, SIZE);

    int numFile = static_cast<int>(msg->TakeByte());

    char fullName[255];
    std::strcpy(fullName, msg->String(2));
    std::strcpy(fullName, "\\");

    if (FileSystem::GetNameFile(msg->String(2), numFile, &fullName[std::strlen(fullName)]))
    {
        float values[4096];
        if (FileSystem::ReadFloats(values, &fullName[1]))
        {
            FillPicture(data, SIZE, values);
        }
    }

    Message::FDrive::PictureDDS(static_cast<uint8>(numFile), data).Transmit();
}


void FileSystem::GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles)
{
    FILINFO fno;
    DIR dir;

    *numDirs = 0;
    *numFiles = 0;

    char nameDir[_MAX_LFN + 1];
    std::memcpy(nameDir, const_cast<char *>(fullPath), std::strlen(fullPath));
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


bool FileSystem::ReadFloats(float values[4096], char *name)
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

    return result;
}


void DDrive::Normalize(float d[4096])
{
    float min = 0.0F;
    float max = 0.0F;

    FindMinMax(d, &min, &max);

    float scale = FindScale(min, max);

    ToScale(d, scale);
}


void DDrive::FindMinMax(const float d[4096], float *_min, float *_max)
{
    float min = 0.0F;
    float max = 0.0F;

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


float DDrive::FindScale(float min, float max)
{
    max = std::fabsf(max);

    if (std::fabsf(min) > max)
    {
        max = std::fabsf(min);
    }

    return 1.0F / max;
}


void DDrive::ToScale(float d[4096], float scale)
{
    for (int i = 0; i < 4096; i++)
    {
        d[i] *= scale;
    }
}


void DDrive::FillPicture(uint8 *picture, uint size, float values[4096])
{
    Normalize(values);

    uint8 aveValue = 127;

    float step = 4096.0F / size;

    for (uint i = 0; i < size; i++)
    {
        float val = values[static_cast<int>(i * step)];

        picture[i] = static_cast<uint8>(aveValue + val * 125);
    }
}
