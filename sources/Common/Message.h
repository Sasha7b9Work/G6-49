#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Message
{
public:
    Message();

    ~Message();

    bool CreateFromMessage(Message *message);
    /// Создать сообщение с выделением памяти
    bool CreateAllocate(uint8 *buffer, uint size);
    /// Создать сообщение без выделения памяти. Используется непосредственно входной буфер
    bool CreateUsed(uint8 *buffer, uint size);
    /// Выделить необходимое количество памяти
    bool AllocateMemory(uint size);
    /// Положить байт
    void Put8(uint8 data);
    /// Положить слово
    void Put32(uint data);
    /// Взять байт
    uint8 Take8();
    /// Взять слово
    uint Take32();
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

private:
    /// Размер выделенной памяти
    uint allocated;
    /// Адрес начала данных, либо 0, если память не выделена
    uint8 *buffer;
    /// Сколько памяти занято реальными данными. Используется для Put
    uint used;
    /// На какой позиции указатель. Используется для Take
    uint taken;
};
