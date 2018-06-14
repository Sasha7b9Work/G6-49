#pragma once
#include "GeneratorSettingsTypes.h"
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Multiplexor
{
public:
    /// Первоначальная инициализация
    static void Init();
    /// Скоммутировать мультиплексор в соответствии с формой устанавливаемого сигнала на данном канале
    static void SetMode(Channel ch, Type_WaveForm form);

private:
    static void SetPin(uint16 pin);
    static void ResetPin(uint16 pin);
};
