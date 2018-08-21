#pragma once
#include "GeneratorSettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AD9952
{
public:

    static void Init();

    static void SetFrequency(Chan ch, float frequency);

    static void SetAmplitude(Chan ch, float amplitude);

private:
    enum
    {
        CFR1,
        CFR2,
        ASF,
        ARR,
        FTW0,
        POW
    };

    static void WriteRegister(Chan ch, uint8 reg);
    static void WriteCFR1(Chan ch);
    static void WriteCFR2(Chan ch);
    static void WriteASF(Chan ch);
    static void WriteFTW0(Chan ch);
};
