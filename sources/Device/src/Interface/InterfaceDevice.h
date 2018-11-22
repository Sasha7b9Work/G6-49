#pragma once
#include "Command.h"
#include "Message.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    static void Update();
    /// Функция запрашивает новое сообщение, которое будет помещено в очередь для отправки. Вызывающая функция должа в месте вызова заполнить его информацией
    static void AddMessageInQueue(Message *message);

    static void SendData(Message *);
    /// Создаёт сообщение для передачи в случае наличия данных для передачи
    static bool CreateMessageForSend(Message *message);

    static void Empty(Message *);

    static void SetFormWave(Message *);

    static void ParameterValue(Message *);

    static void EnableChannel(Message *);

    static void RunReset(Message *);

    static void ModeDebug(Message *);

    static void WriteRegister(Message *);

    static void SetManipulation(Message *);

    static void SetManipulationMode(Message *);

    static void SetStartMode(Message *);

    static void LoadFormDDS(Message *);
    
    static void Test(Message *);

    static void SetKoeffCalibration(Message *);

    static void GetKoeffCalibration(Message *);

    static void SetPolarity(Message *);

    static void Send(void *data, uint size);

private:

    static uint timeLastReceive;
};
