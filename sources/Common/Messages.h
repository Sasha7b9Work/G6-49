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


class SimpleMessage  // -V690
{
public:
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

public:
    static int created;
    static int destroed;
    static int createdSize;
    static int destroedSize;
};


namespace Message
{

    // Служебное сообщение - для обмена между процессорами
    class RequestData : public SimpleMessage
    {
    public:
        RequestData();
    };


    // Сообщение в консоль
    class Log : public SimpleMessage
    {
    public:
        Log(char *string);
    };


    // Установка режима запуска
    class StartMode : public SimpleMessage
    {
    public:
        StartMode(uint8 ch, uint8 mode);
    };


    // Запись в регистр
    class WriteRegister : public SimpleMessage
    {
    public:
        WriteRegister(uint8 reg, uint64 data);
    };


    // Установка отладочного режима
    class DebugMode : public SimpleMessage
    {
    public:
        // mode == 1/0 - включить/отключить режим отладки
        DebugMode(uint8 mode);
    };


    // Включить/выключить канал
    class EnableChannel : public SimpleMessage
    {
    public:
        // ch : 0 - A, 1 - B
        // enable : 0 - выключено, 1 - включено
        EnableChannel(uint8 ch, uint8 enable);
    };

    // Выставить порт регистра
    class RegCPU : public SimpleMessage
    {
    public:
        RegCPU(uint8 port, uint8 pin, uint8 state);
    };


    class Debug : public SimpleMessage
    {
    public:

        Debug();
    };


    class SetFreqMeterTrigK : public SimpleMessage
    {
    public:
        SetFreqMeterTrigK(int16 k);
    };


    namespace FDrive
    {

        // Загрузить выбранный файл в FPGA
        class LoadFromExtStorage : public SimpleMessage
        {
        public:
            LoadFromExtStorage(uint8 ch, uint8 numFile, char *directory);
        };


        // Запрос имени файла
        class FileName : public SimpleMessage
        {
        public:
            FileName(uint8 numFile, char *name);
        };


        class FileString : public SimpleMessage
        {
        public:
            FileString(uint numString, char *nameFile);
        };


        // Число каталогов и файлов в текущем каталоге
        class NumDirsAndFiles : public SimpleMessage
        {
        public:
            NumDirsAndFiles(uint numDirs, uint numFiles);
            NumDirsAndFiles(char *directory);
        };


        // Сообщение о примонтировании/отмонтировании диска
        class Mount : public SimpleMessage
        {
        public:
            // mount :
            // 0 - флешка отмонтирована;
            // 1 - флешка удачно примонтирована
            // 2 - флешку прмонитровать не удалось
            Mount(uint8 mount);
        };


        // Запрос размера файла
        class FileSize : public SimpleMessage
        {
        public:
            FileSize(uint8 numFile, uint size);
        };


        class RequestPictureDDS : public SimpleMessage
        {
        public:
            // Запрос картинки с изображением из файла numberFile
            RequestPictureDDS(uint8 numberFile);

            // Ответ
            RequestPictureDDS(uint8 numberFile, uint8 *data);
        };
    }

    namespace Set
    {

        // Установка формы сигнала
        class FormWave : public SimpleMessage
        {
        public:
            FormWave(uint8 ch, uint8 form);
        };


        // Установка амплитуды
        class Amplitude : public SimpleMessage
        {
        public:
            Amplitude(uint8 ch, uint64 amplitude);
        };


        // Установка смещения сигнала на канале
        class Offset : public SimpleMessage
        {
        public:
            Offset(uint8 ch, uint64 offset);
        };


        // Универсальное сособщение для засылки параметра
        class Parameter : public SimpleMessage
        {
        public:
            Parameter(Command::E param, uint8 ch, uint64 value);
            Parameter(Command::E param, uint8 ch, uint8 choice);
        };
    }

    namespace FreqMeter
    {

        // Сообщение измерения частотомера
        class Measure : public SimpleMessage
        {
        public:
            Measure(uint frequency);
        };
    }

    namespace SCPI
    {
        // Данные SCPI
        class Data : public SimpleMessage
        {
        public:
            Data(uint8 *data, int length);
        };
    }

    // Засылка калибровочного коэффициента и установка соотвествующих настроек генератора
    class CalibrateSet : public SimpleMessage
    {
    public:
        CalibrateSet(uint8 ch, uint8 signal, uint8 range, uint8 parameter);
    };

    // Засылка калибровочного коэффициента
    class CalibrateLoad : public SimpleMessage
    {
    public:
        CalibrateLoad(uint8 ch, uint8 signal, uint8 range, uint8 parameter);
    };
}
