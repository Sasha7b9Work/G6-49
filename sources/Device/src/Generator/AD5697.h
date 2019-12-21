#pragma once
#include "Command.h"
#include "defines.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"


struct AD5697
{
    static void Init();

    static void SetOffset(Chan::E ch, ParamValue offset);

    static void SetFreqLevel(float level);

    static void SetFreqHysteresys(float hyst);

private:
    static void Reset(Chan::E ch);
    /// Запись непосредственно в DAC
    static void TransmitI2C(uint8 address, uint8 data[3]);
    /// Записать три байта данных по данному адресу
    static void WriteParameter(uint8 address, uint8 data[3], HPort::E port, uint16 pin);

    static StructPIN PinLDAC(Chan::E ch);

    static uint8 CreateCommandByte(ParameterValue param);
};
