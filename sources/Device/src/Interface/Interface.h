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

    static void CommandEmpty();

    static void CommandEnable();

    static void CommandReadData();

    static void CommandFormWave();

    static void CommandParameter();

    static void CommandReset();

    static void CommandModeDebug();

    static void CommandWriteRegister();
    /// Команда обрабатывает засылку произвольного сигнала
    static void CommandCreateWave();

    static void CommandSetWave();

    static void CommandSetRampForSine();

    static void CommandSetDurationForRampSine();

    static void CommandSetAmplitudeRampForSine();

private:
    /// Возвращает значение типа float, хранящееся в массиве
    static float GetFloat(uint8 buffer[4]);
    /// Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    static uint freqForSend;

    static uint timeLastReceive;
};
