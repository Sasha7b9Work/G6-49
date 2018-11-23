#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Message
{
public:
    Message();
    Message(uint size, uint8);
    Message(uint size, uint8, uint8);
    Message(uint size, uint8, uint);
    Message(uint size, uint8, uint, uint);
    Message(uint size, uint8, uint8, uint8);
    Message(uint size, uint8, uint8, float);

    ~Message();

    void Create(uint size, uint8);
    void Create(uint size, uint8, uint8);
    void Create(uint size, uint8, uint);
    void Create(uint size, uint8, uint, uint);
    void Create(uint size, uint8, uint8, uint8);
    void Create(uint size, uint8, uint8, float);
    void Create(uint size, uint8, uint8, uint8, uint16);
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
    uint8 *Data();
    /// Возвращает размер данных
    uint Size() const;
    /// Возвращает true, если это null-пакет (память не выделена)
    bool IsEmpty() const;
    /// Возвращает true, если сообщения одинаковы
    bool IsEquals(const Message *message) const;

    /// Положить байт
    void PutByte(uint8 data);
private:
    /// Положить половину слова
    void PutHalfWord(int16 data);
    void PutHalfWord(uint16 data);
    /// Положить слово
    void PutWord(uint data);
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
};
