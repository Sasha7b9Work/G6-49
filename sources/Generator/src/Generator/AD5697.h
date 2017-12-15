#pragma once
#include "../../Common/Command.h"
#include "Common.h"
#include "defines.h"
#include "GeneratorSettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AD5697
{
public:

    void Init();

    void SetAmplitude(Channel ch, float amplitude);

    void SetOffset(Channel ch, float offset);

private:

    void Reset(Channel ch);
    /// Запись непосредственно в DAC
    void TransmitI2C(uint8 address, uint8 data[3]);

    void WriteParameter(Channel ch, WaveParameter param, float value_);

    GeneratorPin PinLDAC(Channel ch);

    uint8 CreateCommandByte(WaveParameter param);
};
