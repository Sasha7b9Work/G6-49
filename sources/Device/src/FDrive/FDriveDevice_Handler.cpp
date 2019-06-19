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
namespace FDrive
{
    namespace FileSystem
    {
        uint GetFileSize(char *fullPath);
        /// Получает количество каталогов и файлов в данной директории
        void GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles);
        /// Получить имя numFile-го файла из каталога fullPath
        bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut);
        /// Читает значения отсчётов сигнала из файла name
        bool ReadFloats(float values[4096], char *name);
    }

    namespace Handler
    {
        SimpleMessage *msg = nullptr;
        void GetNumDirsAndFiles();
        void RequestFile();
        void RequestFileSize();
        /// Загрузить сигнал с флешки
        void LoadFromExtStorage();
        void GetPictureDDS();
        void E();
    }

    /// Трансформировать точки в пригодный для записи в ПЛИС вид
    void TransformDataToCode(float d[4096], uint8 code[FPGA::NUM_POINTS * 2]);

    void Normalize(float d[4096]);

    void FindMinMax(float d[4096], float *_min, float *_max);

    float FindScale(float min, float max);

    void ToScale(float d[4096], float scale);
    /// Заполнить массив picture данными для отрисовки сигнала на экране
    void FillPicture(uint8 *picture, uint size, float values[4096]);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FDrive::Handler::Processing(SimpleMessage *message)
{
    msg = message;

    msg->ResetPointer();

    uint8 com = msg->TakeByte();

    static const pFuncVV funcs[Command::Number] =
    {
        /* RequestData                  */ EmptyFuncVV,
        /* EnableChannel                */ EmptyFuncVV,
        /* SetFormWave                  */ EmptyFuncVV,
        /* SetFrequency                 */ EmptyFuncVV,
        /* SetAmplitude                 */ EmptyFuncVV,
        /* SetOffset                    */ EmptyFuncVV,
        /* SetDuration                  */ EmptyFuncVV,
        /* SetDutyRatio                 */ EmptyFuncVV,
        /* SetPhase                     */ EmptyFuncVV,
        /* RunReset                     */ EmptyFuncVV,
        /* ModeDebug                    */ EmptyFuncVV,
        /* SetDelay                     */ EmptyFuncVV,
        /* WriteRegister                */ EmptyFuncVV,
        /* SetDurationRise              */ EmptyFuncVV,
        /* SetDurationFall              */ EmptyFuncVV,
        /* SetDurationStady             */ EmptyFuncVV,
        /* SetDutyFactor                */ EmptyFuncVV,
        /* SetManipulation              */ EmptyFuncVV,
        /* SetManipulationDuration      */ EmptyFuncVV,
        /* SetManipulationPeriod        */ EmptyFuncVV,
        /* SetPacketPeriod              */ EmptyFuncVV,
        /* SetPacketNumber              */ EmptyFuncVV,
        /* SetStartMode                 */ EmptyFuncVV,
        /* SetPeriod                    */ EmptyFuncVV,
        /* SetPolarity                  */ EmptyFuncVV,
        /* SetManipulationMode          */ EmptyFuncVV,
        /* LoadFromDDS                  */ EmptyFuncVV,
        /* FreqMeasure                  */ EmptyFuncVV,
        /* Log                          */ EmptyFuncVV,
        /* FDrive_NumDirsAndFiles       */ Handler::GetNumDirsAndFiles,
        /* FDrive_Mount                 */ Handler::E,
        /* FDrive_RequestDir            */ Handler::E,
        /* FDrive_RequestFile           */ Handler::RequestFile,
        /* Test                         */ EmptyFuncVV,
        /* SetKoeffCalibration          */ EmptyFuncVV,
        /* GetKoeffCalibration          */ EmptyFuncVV,
        /* FDrive_RequestFileSize       */ Handler::RequestFileSize,
        /* FDrive_RequestFileString     */ Handler::E,
        /* FDrive_LoadFromExtStorage    */ Handler::LoadFromExtStorage,
        /* FDrive_GetPictureDDS         */ Handler::GetPictureDDS,
        /* SCPI_RecvData                */ EmptyFuncVV
    };

    funcs[com]();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Handler::GetNumDirsAndFiles()
{
    uint numDirs = 0;
    uint numFiles = 0;

    FileSystem::GetNumDirsAndFiles(msg->String(1), &numDirs, &numFiles);

    Message::FDrive::NumDirsAndFiles(numDirs, numFiles).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Handler::RequestFile()
{
    char name[255];

    int numFile = (int)msg->TakeByte();

    if (FileSystem::GetNameFile(msg->String(2), numFile, name))
    {
        Message::FDrive::FileName((uint8)numFile, name).Transmit();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Handler::RequestFileSize()
{
    char name[255];
    int numFile = (int)msg->TakeByte();
    if (FileSystem::GetNameFile(msg->String(2), numFile, name))           // Получаем имя файла
    {
        String fullPath("%s\\%s", msg->String(2), name);

        uint size = FileSystem::GetFileSize(fullPath.CString());

        Message::FDrive::FileSize((uint8)numFile, size).Transmit();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Handler::LoadFromExtStorage()
{
    char fullName[255];
    Chan::E ch = (Chan::E)msg->TakeByte();
    int numFile = (int)msg->TakeByte();
    std::strcpy(fullName, msg->String(2));
    std::strcat(fullName, "\\");
    if (FileSystem::GetNameFile(msg->String(2), numFile, &fullName[std::strlen(fullName)]))
    {
        Buffer buffer(Generator::DDS_NUM_POINTS * sizeof(float));
        FileSystem::ReadFloats(buffer.DataFloat(), &fullName[1]);
        uint8 code[Generator::DDS_NUM_POINTS * 2];
        TransformDataToCode(buffer.DataFloat(), code);
        FPGA::SaveExtSignal(ch, code);

        Message::FDrive::LoadFromExtStorage(ch, numFile, 0);    // Посылаем признак того, что сохранение завершено
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Handler::GetPictureDDS()
{
    const uint SIZE = 240;
    uint8 data[SIZE];
    std::memset(data, 0, SIZE);

    int numFile = (int)msg->TakeByte();

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

    Message::FDrive::PictureDDS((uint8)numFile, data).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Handler::E()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::FileSystem::GetNumDirsAndFiles(const char *fullPath, uint *numDirs, uint *numFiles)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::FileSystem::GetNameFile(const char *fullPath, int numFile, char *nameFileOut)
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
uint FDrive::FileSystem::GetFileSize(char *fullPath)
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
bool FDrive::FileSystem::ReadFloats(float values[4096], char *name)
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
void FDrive::TransformDataToCode(float d[4096], uint8 code[FPGA::NUM_POINTS * 2])
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
void FDrive::Normalize(float d[4096])
{
    float min = 0.0f;
    float max = 0.0f;

    FindMinMax(d, &min, &max);

    float scale = FindScale(min, max);

    ToScale(d, scale);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::FindMinMax(float d[4096], float *_min, float *_max)
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
float FDrive::FindScale(float min, float max)
{
    max = std::fabsf(max);

    if (std::fabsf(min) > max)
    {
        max = std::fabsf(min);
    }

    return 1.0f / max;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::ToScale(float d[4096], float scale)
{
    for (int i = 0; i < 4096; i++)
    {
        d[i] *= scale;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::FillPicture(uint8 *picture, uint size, float values[4096])
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
