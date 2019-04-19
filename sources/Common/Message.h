#pragma once
#include "Command.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimpleMessage  // -V690
{
    friend class Transceiver;
public:
    explicit SimpleMessage();
    explicit SimpleMessage(uint size, uint8);
    explicit SimpleMessage(uint size, uint8, uint8);
    explicit SimpleMessage(uint size, uint8, uint8, uint);
    explicit SimpleMessage(uint size, uint8, uint, uint);
    explicit SimpleMessage(uint size, uint8, uint8, uint8);
    explicit SimpleMessage(uint size, uint8, uint8, float);
    explicit SimpleMessage(uint size, uint8, uint8, uint64);
    explicit SimpleMessage(uint8, char*);
    explicit SimpleMessage(uint8, uint8, char *);
    explicit SimpleMessage(uint8, uint8, uint8, char *);
    explicit SimpleMessage(uint8, uint, char*);

    ~SimpleMessage();

    void Create(uint size, uint8);
    void Create(uint size, uint8, uint8);
    void Create(uint size, uint8, uint, uint);
    void Create(uint size, uint8, uint8, uint8);
    void Create(uint size, uint8, uint8, float);
    void Create(uint size, uint8, uint8, uint8, uint16);
    void Create(uint8, uint8, uint8, char *string);
    void Create(uint8, uint8, char *string);
    void Create(uint8, uint, char *);
    void Create(uint size, uint8 com, uint8 d0, uint d1);
    /// Возвращает указатель на созданную копию
    SimpleMessage *Clone();
    /// Сбрасывает указатель извлечённой информации.
    void ResetPointer() { taken = 0; };

    bool CreateFromMessage(SimpleMessage *message);
    /// Создать сообщение с выделением памяти
    bool CreateAllocate(uint8 *buffer, uint size);
    /// Выделить необходимое количество памяти
    bool AllocateMemory(uint size);
    /// Взять байт
    uint8 TakeByte();
    /// Взять полуслово
    uint16 TakeHalfWord();
    /// Взять слово
    uint TakeWord();

    uint64 TakeDoubleWord();

    float TakeFloat();
    /// Взять оставшиеся данные. После вызова этой функции забирать из сообщения больше нечего
    void TakeRemainigData(uint8 *data);
    /// Возвращает указатель на оставшиеся данные. Данные можно забрать потом
    uint8 *RemainingData() const;
    /// Освободить память
    void FreeMemory();
    /// Возвращает указатель на данные
    uint8 *Data(int pos = 0);
    /// Возвращает размер данных
    uint Size() const;
    /// Возвращает true, если это null-пакет (память не выделена)
    bool IsEmpty() const;
    /// Возвращает true, если сообщения одинаковы
    bool IsEquals(const SimpleMessage *message) const;
    /// Возвращает указатель на pos-й элемент. Удобно для копирования строк
    char *String(int pos);

    pString Trace();
    /// Положить слово
    void PutWord(uint data);
    /// Положить данные
    void PutData(uint8 *data, uint length);
    /// Передать сообщение получателю
    void Transmit();

protected:
    /// Положить байт
    void PutByte(uint8 data);
    /// Положить половину слова
    void PutHalfWord(int16 data);

    void PutDoubleWord(uint64 data);

    void PutHalfWord(uint16 data);
    /// Положить float
    void PutFloat(float data);
    /// Размер выделенной памяти
    uint allocated;
    /// Адрес начала данных, либо 0, если память не выделена
    uint8 *buffer;
    /// Сколько памяти занято реальными данными. Используется для Put
    uint used;
    /// На какой позиции указатель. Используется для Take
    uint taken;

    SimpleMessage(const SimpleMessage &) : allocated(0), buffer(0), used(0), taken(0) {};
};


namespace Message
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Служебное сообщение - для обмена между процессорами
    class RequestData : public SimpleMessage
    {
    public:
        RequestData();
    };

    namespace FDrive
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Загрузить выбранный файл в FPGA
        class LoadToFPGA : public SimpleMessage
        {
        public:
            LoadToFPGA(uint8 ch, uint8 numFile, char *directory);
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Запрос имени файла
        class FileName : public SimpleMessage
        {
        public:
            FileName(uint8 numFile, char *name);
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Число каталогов и файлов в текущем каталоге
        class NumDirsAndFiles : public SimpleMessage
        {
        public:
            NumDirsAndFiles(uint numDirs, uint numFiles);
        };
    }

    namespace Set
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Установка формы сигнала
        class FormWave : public SimpleMessage
        {
        public:
            FormWave(uint8 ch, uint8 form);
        };
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Включить/выключить канал
class MessageEnableChannel : public SimpleMessage
{
public:
    /// ch : 0 - A, 1 - B
    /// enable : 0 - выключено, 1 - включено
    MessageEnableChannel(uint8 ch, uint8 enable);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установка частоты
class MessageSetFrequency : public SimpleMessage
{
public:
    MessageSetFrequency(uint8 ch, uint64 frequency);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установка амплитуды
class MessageSetAmplitude : public SimpleMessage
{
public:
    MessageSetAmplitude(uint8 ch, uint64 amplitude);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установка смещения сигнала на канале
class MessageSetOffset : public SimpleMessage
{
public:
    MessageSetOffset(uint8 ch, uint64 offset);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Сообщение измерения частотомера
class MessageFreqMeasure : public SimpleMessage
{
public:
    MessageFreqMeasure(uint frequency);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Сообщение о примонтировании/отмонтировании диска
class MessageFDriveMount : public SimpleMessage
{
public:
    /// mount :
    /// 0 - флешка отмонтирована;
    /// 1 - флешка удачно примонтирована
    /// 2 - флешку прмонитровать не удалось
    MessageFDriveMount(uint8 mount);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Запрос размера файла
class MessageFDriveFileSize : public SimpleMessage
{
public:
    MessageFDriveFileSize(uint8 numFile, uint size);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Сообщение в консоль
class MessageLog : public SimpleMessage
{
public:
    MessageLog(char *string);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установка режима запуска
class MessageStartMode : public SimpleMessage
{
public:
    MessageStartMode(uint8 ch, uint8 mode);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Запись в регистр
class MessageWriteRegister : public SimpleMessage
{
public:
    MessageWriteRegister(uint8 reg, uint64 data);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установка отладочного режима
class MessageDebugMode : public SimpleMessage
{
public:
    /// mode == 1/0 - включить/отключить режим отладки
    MessageDebugMode(uint8 mode);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Сброс состояния
class MessageReset : public SimpleMessage
{
public:
    MessageReset();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Универсальное сособщение для засылки параметра
class MessageParameter : public SimpleMessage
{
public:
    MessageParameter(Command::E param, uint8 ch, uint64 value);
};
