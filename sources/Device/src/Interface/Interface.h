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

    static SPI_HandleTypeDef *HandleSPI();

    static void ReceiveCallback();

    static void Empty();

    static void Enable();

    static void ReadData();

    static void FormWave();

    static void ParameterValue();

    static void Reset();

    static void ModeDebug();

    static void WriteRegister();
    /// Команда обрабатывает засылку произвольного сигнала
    static void CreateWave();

    static void SetManipulation();

    static void SetManipulationMode();

    static void SetStartMode();

    static void Polarity();

private:
    /// Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    static uint freqForSend;

    static uint timeLastReceive;
};
