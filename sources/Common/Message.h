#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Message																	// -V690
{
public:
    explicit Message();
    explicit Message(uint size, uint8);
    explicit Message(uint size, uint8, uint8);
    explicit Message(uint size, uint8, uint);
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
    void Create(uint size, uint8, uint);
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

private:
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
