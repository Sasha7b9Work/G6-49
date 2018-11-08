#pragma once
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    static void Init();

    static void Update();
    /// Послать в панель измеренное значение частоты
    static void SendFrequency(uint frequency);

    static void Empty();

    static void SetFormWave();

    static void ParameterValue();

    static void EnableChannel();

    static void RunReset();

    static void ModeDebug();

    static void WriteRegister();
    /// Команда обрабатывает засылку произвольного сигнала
    static void CreateWave();

    static void SetManipulation();

    static void SetManipulationMode();

    static void SetStartMode();

    static void LoadFormDDS();

    static void SetPolarity();

    static void Send(void *data, uint size);
    ///< Буфер для принимаемых команд
    static uint8 *recv;

private:
    /// Установить размер приёмного буфера равным size
    static void ResizeRecieveBuffer(uint16 size);

    static uint timeLastReceive;
};
