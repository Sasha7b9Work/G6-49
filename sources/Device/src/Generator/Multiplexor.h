#pragma once
#include "GeneratorSettingsTypes.h"
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Multiplexor
{
friend class Interface;
public:

    /// Первоначальная инициализация
    static void Init();
    /// Скоммутировать мультиплексор в соответствии с формой устанавливаемого сигнала на данном канале
    static void SetMode(Chan ch, Form form);

private:
    static void SetPin(uint16 pin);
    static void ResetPin(uint16 pin);

    static void WriteRegister(Register reg, uint value);
};
