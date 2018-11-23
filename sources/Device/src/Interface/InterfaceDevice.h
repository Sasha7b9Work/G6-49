#pragma once
#include "Command.h"
#include "Message.h"
#include "Utils/Queue.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interface
{
    friend class Handlers;

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

    static void Update();
    /// Добавляет сообщение в очередь отправки. Если очередь заполнена, сообщение удаляется и функция возвращает false
    static bool AddMessageInQueue(Message *message);

private:

    static uint timeLastReceive;
    /// Очередь сообщений, ожидающих отправки
    static Queue messages;
    /// Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    static uint freqForSend;
};
