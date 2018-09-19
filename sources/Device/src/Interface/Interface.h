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

    static void Parameter();

    static void Reset();

    static void ModeDebug();

    static void WriteRegister();
    /// Команда обрабатывает засылку произвольного сигнала
    static void CreateWave();

    static void SetRampForSine();

    static void SetDurationForRampSine();

    static void SetAmplitudeRampForSine();

    static void SetPacketPeriod();

    static void SetPacketNumber();

private:

    /// Возвращает значение типа float, хранящееся в массиве
    static float GetFloat(uint8 buffer[4]);
    /// Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    static uint freqForSend;

    static uint timeLastReceive;
};
