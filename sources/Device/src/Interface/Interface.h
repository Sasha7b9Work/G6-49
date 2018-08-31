#pragma once
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interface
{
public:

    static void Init();

    static void Update();
    /// ѕослать в панель измеренное значение частоты
    static void SendFrequency(uint frequency);

    static SPI_HandleTypeDef *HandleSPI();

    static void ReceiveCallback();

private:

    static void CommandEmpty();

    static void CommandEnable();

    static void CommandReadData();

    static void CommandFormWave();

    static void CommandParameter();

    static void CommandReset();

    static void CommandModeDebug();

    static void CommandWriteRegister();
    ///  оманда обрабатывает засылку произвольного сигнала
    static void CommandCreateWave();

    static void CommandSetWave();
    /// Ќенулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    static uint freqForSend;

    typedef void(*pFuncInterfaceVV)();

    struct FuncInterface
    {
        pFuncInterfaceVV func;
        FuncInterface(pFuncInterfaceVV f) : func(f) {};
    };
    /// «десь хран€тс€ обработчики команд
    static const FuncInterface commands[CommandPanel::Number];

    static uint timeLastReceive;
};
