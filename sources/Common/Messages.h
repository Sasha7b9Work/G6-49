// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Command.h"

#ifdef PANEL
#include "Generator/Generator_p.h"
#else
#ifdef PLOADER
#include "Generator/Generator_p.h"
#else
#include "Generator/Generator_d.h"
#endif
#endif


struct SimpleMessage  // -V690
{
    explicit SimpleMessage();
    explicit SimpleMessage(int size, uint8);
 
    ~SimpleMessage();

    void Create(int size, uint8);
    // Возвращает указатель на созданную копию
    SimpleMessage *Clone();
    // Сбрасывает указатель извлечённой информации.
    void ResetPointer() { taken = 0; };

    bool CreateFromMessage(const SimpleMessage *message);
    // Создать сообщение с выделением памяти
    bool CreateAllocate(const uint8 *buffer, int size);
    // Выделить необходимое количество памяти
    bool AllocateMemory(int size);
    // Взять оставшиеся данные. После вызова этой функции забирать из сообщения больше нечего
    void TakeRemainigData(uint8 *data);
    // Возвращает указатель на оставшиеся данные. Данные можно забрать потом
    uint8 *RemainingData() const;
    // Освободить память
    void FreeMemory();
    // Возвращает размер данных
    int Size() const;
    // Возвращает true, если это null-пакет (память не выделена)
    bool IsEmpty() const;
    // Возвращает true, если сообщения одинаковы
    bool IsEquals(const SimpleMessage *message) const;
    // Возвращает указатель на pos-й элемент. Удобно для копирования строк
    char *String(int pos);

    void PutUINT8(uint8 data);
    void PutINT16(int16 data);
    void PutUINT16(uint16 data);
    void PutINT(int data);
    void PutUINT(uint data);
    void PutUINT64(uint64 data);
    void PutFLOAT(float data);
    void PutData(const uint8 *data, int length);

    uint8  TakeUINT8();
    int16  TakeINT16();
    uint16 TakeUINT16();
    int    TakeINT();
    uint   TakeUINT();
    uint64 TakeUINT64();
    float  TakeFLOAT();
    uint8 *TakeData(int pos = 0);

    // Передать сообщение получателю
    void Transmit();
    // Передать сообщение получателю и дождаться, пока оно покинет передаточный буфер
    void TransmitAndSend();

protected:

    // Размер выделенной памяти
    int allocated;
    // Адрес начала данных, либо 0, если память не выделена
    uint8 *buffer;
    // Сколько памяти занято реальными данными. Используется для Put
    int used;
    // На какой позиции указатель. Используется для Take
    int taken;

    SimpleMessage(const SimpleMessage &) : allocated(0), buffer(0), used(0), taken(0) {};
    SimpleMessage &operator=(const SimpleMessage &);

public:
    static int created;
    static int destroed;
    static int createdSize;
    static int destroedSize;
};


namespace Message
{

    // Служебное сообщение - для обмена между процессорами
    struct RequestData : public SimpleMessage
    {
        RequestData();
    };


    // Сообщение в консоль
    struct Log : public SimpleMessage
    {
        Log(char *string);
    };


    // Установка режима запуска
    struct StartMode : public SimpleMessage
    {
        StartMode(uint8 ch, uint8 mode);
    };


    // Режим Старт/Стоп импульсного сигнала
    struct StartStopMode : public SimpleMessage
    {
        // mode : 0 - автоматический режим
        //        1 - ручной режим
        StartStopMode(int mode);
    };

    // Запись в регистр
    struct WriteRegister : public SimpleMessage
    {
        WriteRegister(uint8 reg, uint64 data);
    };


    // Установка отладочного режима
    struct DebugMode : public SimpleMessage
    {
        // mode == 1/0 - включить/отключить режим отладки
        DebugMode(uint8 mode);
    };


    // Включить/выключить канал
    struct EnableChannel : public SimpleMessage
    {
        // ch : 0 - A, 1 - B
        // enable : 0 - выключено, 1 - включено
        EnableChannel(uint8 ch, uint8 enable);
    };

    struct SingleStart : public SimpleMessage
    {
        SingleStart();
    };

    // Выставить порт регистра
    struct RegCPU : public SimpleMessage
    {
        RegCPU(uint8 port, uint8 pin, uint8 state);
    };


    struct Debug : public SimpleMessage
    {
        Debug();
    };


    struct SetFreqMeterTrigK : public SimpleMessage
    {
        SetFreqMeterTrigK(int16 k);
    };


    namespace FDrive
    {
        // Запрос имени файла
        struct FileName : public SimpleMessage
        {
            FileName(uint8 numFile, char *name);
        };


        struct FileString : public SimpleMessage
        {
            FileString(uint numString, char *nameFile);
        };


        // Число каталогов и файлов в текущем каталоге
        struct NumDirsAndFiles : public SimpleMessage
        {
            NumDirsAndFiles(uint numDirs, uint numFiles);
            NumDirsAndFiles(char *directory);
        };


        // Сообщение о примонтировании/отмонтировании диска
        struct Mount : public SimpleMessage
        {
            // mount :
            // 0 - флешка отмонтирована;
            // 1 - флешка удачно примонтирована
            // 2 - флешку прмонитровать не удалось
            Mount(uint8 mount);
        };


        // Запрос размера файла
        struct FileSize : public SimpleMessage
        {
            FileSize(uint8 numFile, uint size);
        };


        struct RequestPictureDDSfromFile : public SimpleMessage
        {
            // Запрос картинки с изображением из файла numberFile
            RequestPictureDDSfromFile(uint8 numberFile);

            // Ответ
            RequestPictureDDSfromFile(uint8 numberFile, uint8 data[240]);
        };

        // Загрузить выбранный файл в FPGA
        struct LoadDDSfromFile : public SimpleMessage
        {
            LoadDDSfromFile(uint8 ch, uint8 numFile, char *directory);
        };


        struct CreateFFile : public SimpleMessage
        {
            CreateFFile(pchar name);
        };

        struct WriteToFile : public SimpleMessage
        {
            WriteToFile(void *data, int size);
        };

        struct CloseFile : public SimpleMessage
        {
            CloseFile();
        };
    }


    namespace Storage
    {
        struct RequestPictureDDS : public SimpleMessage
        {
            // Запрос изображения произвольного сигнала из ППЗУ
            RequestPictureDDS(uint8 ch);

            // Ответ
            RequestPictureDDS(uint8 ch, uint8 data[240]);
        };

        struct LoadDDS : public SimpleMessage
        {
            LoadDDS();
        };
    }

    namespace Set
    {

        // Установка формы сигнала
        struct FormWave : public SimpleMessage
        {
            FormWave(uint8 ch, uint8 form);
        };


        // Установка амплитуды
        struct Amplitude : public SimpleMessage
        {
            Amplitude(uint8 ch, uint64 amplitude);
        };


        // Установка смещения сигнала на канале
        struct Offset : public SimpleMessage
        {
            Offset(uint8 ch, uint64 offset);
        };


        // Универсальное сособщение для засылки параметра
        struct Parameter : public SimpleMessage
        {
            Parameter(Command::E param, uint8 ch, uint64 value);
            Parameter(Command::E param, uint8 ch, uint8 choice);
        };
    }

    namespace FreqMeter
    {

        // Сообщение измерения частотомера
        struct Measure : public SimpleMessage
        {
            Measure(uint frequency);
        };
    }

    namespace SCPI
    {
        // Данные SCPI
        struct Data : public SimpleMessage
        {
            Data(uint8 *data, int length);
        };
    }

    // Засылка калибровочного коэффициента и установка соотвествующих настроек генератора
    struct CalibrateSet : public SimpleMessage
    {
        CalibrateSet(uint8 ch, uint8 signal, uint8 range, uint8 parameter);
    };

    // Засылка калибровочного коэффициента
    struct CalibrateLoad : public SimpleMessage
    {
        CalibrateLoad(uint8 ch, uint8 signal, uint8 range, uint8 parameter);
    };
}
