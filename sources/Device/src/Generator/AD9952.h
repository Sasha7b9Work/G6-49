#pragma once
#include "defines.h"
#include "Common.h"
#include "Hardware/CPU.h"
#include "GeneratorSettingsTypes.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace AD9952
{
    void Init();

    void SetFrequency(Chan::E ch, ParamValue frequency);

    void SetAmplitude(Chan::E ch, ParamValue amplitude);

    void SetPhase(Chan::E ch, ParamValue phase);

    /// Манипуляция
    namespace Manipulation
    {
        /// True означает, что манипуляция включена
        extern bool enabled[Chan::Count];
        struct Type
        {
            enum E
            {
                OSK,    ///< Манипуляция импульсами со сглаженными фронтами
                FPGA    ///< Манипуляция прямоугольными импульсами
            } value;
        };
        extern Type::E type[Chan::Count];
        /// Установить/отменить модулирование синусоиды сигналом "пилы"
        void SetEnabled(Chan::E ch, bool enable);
        inline bool IsEnabled(Chan::E ch) { return enabled[ch]; };
        void SetType(Chan::E ch, Type::E type);
        inline Type::E GetType(Chan::E ch) { return type[ch]; };
    };
};
