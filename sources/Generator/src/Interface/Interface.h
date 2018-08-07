#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interface
{
public:

    static void Init();

    static void ProcessingCommand();
    /// Послать в панель измеренное значение частоты
    static void SendFrequency(uint frequency);

private:

    static void CommandEmpty();

    static void ProcessCommand();

    static void CommandEnable();

    static void CommandReadData();

    static void CommandFormWave();

    static void CommandParameter();

    static void CommandReset();

    static void CommandModeDebug();

    static void CommandWriteRegister();

    static void SendToInterface(uint8 *trans);
    /// Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    static uint freqForSend;
};
