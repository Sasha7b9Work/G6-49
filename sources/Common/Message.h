#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Message
{
public:
    Message();

    ~Message();

    bool CreateFromRawData(uint8 *buffer, uint size);
    /// Выделить необходимое количество памяти
    bool AllocateMemory(uint size);
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

    uint size;

    uint8 *buffer;
};
