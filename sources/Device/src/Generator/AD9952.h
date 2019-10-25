#pragma once
#include "defines.h"
#include "Common.h"
#include "Hardware/CPU.h"
#include "GeneratorSettingsTypes.h"



namespace AD9952
{
    void Init();

    void SetFrequency(Chan::E ch, ParamValue frequency);

    void SetAmplitude(Chan::E ch, ParamValue amplitude);

    void SetPhase(Chan::E ch, ParamValue phase);

    /// ћанипул€ци€
    namespace Manipulation
    {
        /// True означает, что манипул€ци€ включена
        extern bool enabled[Chan::Count];
        /// ”становить/отменить модулирование синусоиды сигналом "пилы"
        void SetEnabled(Chan::E ch, bool enable);
        inline bool IsEnabled(Chan::E ch) { return enabled[ch]; };
    };
};
