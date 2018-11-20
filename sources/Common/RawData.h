#pragma once


/*
    Класс для облегения формирования raw-сообщений (сообщений как они описаны в Command.h
*/

class RawData
{
public:
    /// Конструирует сообщение длиной size
    RawData(uint size);
    RawData(uint size, uint8 value0, uint8 valu1);
    ~RawData();

    void Put(uint8 value);
    void Put(float value);

    uint8 *Data() { return buffer; }
    uint Size() const { return used; }

private:

    void Alloc(uint size);
    /// Указатель на начало сообщения
    uint8 *buffer;
    /// Количество байт, выделенных для сообщения
    uint allocated;
    /// Количество реальных байт в сообщении
    uint used;
};
