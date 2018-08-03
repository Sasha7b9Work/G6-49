#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interface
{
public:

    static void Init();

    static void ProcessingCommand();
    /// ѕослать в панель измеренное значение частоты
    static void SendFrequency(uint frequency);

private:

    static void CommandEmpty();

    static void ProcessCommand();

    static void CommandEnable();

    static void CommandFormWave();

    static void CommandParameter();

    static void CommandReset();

    static void CommandModeDebug();

    static void CommandWriteRegister();
};
