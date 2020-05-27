#pragma once
#include "common/Common.h"
#include "common/Command.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"


struct AD5697
{
    static void Init();

    static void SetOffset(Chan::E ch);

    static void SetFreqLevel(double level);

    static void SetFreqHysteresys(double hyst);

private:
    static void Reset(Chan::E ch);

    // «аписать три байта данных по данному адресу
    static void WriteParameter(uint8 address, uint8 data[3], HPort::E port, uint16 pin);

    static StructPIN PinLDAC(Chan::E ch);

    // –ассчитать код смещени€ дл€ смещени€ offset
    static double CalculateCodeOffset(Chan::E ch);
};
