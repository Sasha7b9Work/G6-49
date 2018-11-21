#pragma once
#include "Message.h"
#include "Utils/Buffer.h"


class Interface
{
public:

    /// Структура для работы с интерфейсом
    struct Data
    {
    public:

        Data(uint size, uint8 command = 0);

        ~Data();
        /// Инициализировать структуры для хранения size байт данных
        bool Init(uint size);
        /// Освободить память, использованную структурой. Освобождать сразу же после использования
        void Release();
        /// Возвращает true, если буфер пуст - нулевой размер
        bool IsEmpty() const;
        /// Возвращает указатель на данные
        uint8 *GetData();
        /// Возвращает размер буфера данных
        uint GetSize() const;
    private:
        uint8 *data;
        uint size;
    };

    /// Для приёма сообщений от устройства
    static void Update();
    /// Заслать в генератор данные
    static void Send(uint8 *buffer, uint size);

    static void Send(const Buffer &buffer);

    //static void Send(RawData *message);

    static void Send(Message *message);

private:
    /// Выполняет команду recv
    static bool Run(Message *message);
};
