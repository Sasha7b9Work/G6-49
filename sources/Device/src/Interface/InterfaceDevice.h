#pragma once
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interface
{
public:

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
    ///< Буфер для принимаемых команд
    static uint8 *recv;

private:
    /// Установить размер приёмного буфера равным size
    static void ResizeRecieveBuffer(uint16 size);

    static uint timeLastReceive;
};
