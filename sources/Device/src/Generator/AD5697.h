#pragma once
#include "Command.h"
#include "defines.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace AD5697
{
    void Init();

    void SetOffset(Chan::E ch, ParamValue offset);

    void SetFreqLevel(float level);

    void SetFreqHysteresys(float hyst);
};
