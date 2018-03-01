#pragma once
#include "GeneratorSettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AD9952
{
public:

    static void Init();

    static void SetFrequency(Channel ch, float frequency);

    static void SetAmplitude(Channel ch, float amplitude);

private:
};
