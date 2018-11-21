#pragma once
#include "Command.h"


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
    
    static void UpdateOld();
    /// Послать в панель измеренное значение частоты
    static void SendFrequency(uint frequency);

    static void Empty(uint8 *);

    static void SetFormWave(uint8 *);

    static void ParameterValue(uint8 *);

    static void EnableChannel(uint8 *);

    static void RunReset(uint8 *);

    static void ModeDebug(uint8 *);

    static void WriteRegister(uint8 *);

    static void SetManipulation(uint8 *);

    static void SetManipulationMode(uint8 *);

    static void SetStartMode(uint8 *);

    static void LoadFormDDS(uint8 *);
    
    static void Test(uint8 *);

    static void SetKoeffCalibration(uint8 *);

    static void SetPolarity(uint8 *);

    static void Send(void *data, uint size);

private:

    static uint timeLastReceive;
};
