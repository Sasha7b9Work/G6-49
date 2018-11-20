#pragma once
#include "RawMessage.h"
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

    static void Send(RawMessage *message);
    /// Послать запрос и ожидать ответа.
    static bool Request(Data *request, Data *answer);

private:
    /// Принять и обработать данные от ПЛИС
    static void ProcessDataFPGA();
    /// Принять numBytes байт от ПЛИС и выполнить их
    static void ReceiveAndRun(uint numBytes);
    /// Сколько байт хочет нам передать прибор
    static uint BytesForReceive();
};
