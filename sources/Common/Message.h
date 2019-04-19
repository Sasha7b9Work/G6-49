#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Message																	// -V690
{
    friend class Transceiver;
public:
    explicit Message();
    explicit Message(uint size, uint8);
    explicit Message(uint size, uint8, uint8);
    explicit Message(uint size, uint8, uint8, uint);
    explicit Message(uint size, uint8, uint, uint);
    explicit Message(uint size, uint8, uint8, uint8);
    explicit Message(uint size, uint8, uint8, float);
    explicit Message(uint size, uint8, uint8, uint64);
    explicit Message(uint8, char*);
    explicit Message(uint8, uint8, char *);
    explicit Message(uint8, uint8, uint8, char *);
    explicit Message(uint8, uint, char*);

    ~Message();

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
    Message *Clone();
    /// Сбрасывает указатель извлечённой информации.
    void ResetPointer() { taken = 0; };

    bool CreateFromMessage(Message *message);
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
    bool IsEquals(const Message *message) const;
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

    Message(const Message &) {};
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Служебное сообщение - для обмена между процессорами
class MessageRequestData : public Message
{
public:
    MessageRequestData();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Включить/выключить канал
class MessageEnableChannel : public Message
{
public:
    /// ch : 0 - A, 1 - B
    /// enable : 0 - выключено, 1 - включено
    MessageEnableChannel(uint8 ch, uint8 enable);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установка формы сигнала
class MessageSetFromWave : public Message
{
public:
    MessageSetFromWave(uint8 ch, uint8 form);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установка частоты
class MessageSetFrequency : public Message
{
public:
    MessageSetFrequency(uint8 ch, uint64 frequency);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установка амплитуды
class MessageSetAmplitude : public Message
{
public:
    MessageSetAmplitude(uint8 ch, uint64 amplitude);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установка смещения сигнала на канале
class MessageSetOffset : public Message
{
public:
    MessageSetOffset(uint8 ch, uint64 offset);
};





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Сообщение измерения частотомера
class MessageFreqMeasure : public Message
{
public:
    MessageFreqMeasure(uint frequency);
};

