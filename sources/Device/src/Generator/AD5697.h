#pragma once
#include "Command.h"
#include "defines.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AD5697
{
public:

    static void Init();

    static void SetOffset(Chan ch, float offset);

    static void SetFreqLevel(float level);

    static void SetFreqHysteresys(float hyst);

private:

    static void Reset(Chan ch);
    /// Запись непосредственно в DAC
    static void TransmitI2C(uint8 address, uint8 data[3]);

    //static void WriteParameter(Chan ch, Wave::Form::Parameter param, float value_);
    /// Записать три байта данных по данному адресу
    static void WriteParameter(uint8 address, uint8 data[3], GeneratorWritePin pin);

    static GeneratorWritePin PinLDAC(Chan ch);

    static uint8 CreateCommandByte(Wave::Form::Parameter param);
};
