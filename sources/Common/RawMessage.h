#pragma once


/*
    Класс для облегения формирования raw-сообщений (сообщений как они описаны в Command.h
*/

class RawMessage
{
public:
    /// Конструирует сообщение длиной size
    RawMessage(uint size);
    ~RawMessage();
private:
    /// Указатель на начало сообщения
    uint8 *buffer;
    /// Количество байт, выделенных для сообщения
    uint allocated;
    /// Количество реальных байт в сообщении
    uint used;
};
