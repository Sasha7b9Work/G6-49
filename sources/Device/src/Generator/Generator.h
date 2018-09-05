#pragma once
#include "defines.h"
#include "AD9952.h"
#include "AD5697.h"
#include "FPGA.h"
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Generator
{
public:
    static void Init();

    static void EnableChannel(Chan ch, bool enable);

    static void SetFormWave(Chan ch, Wave::Form form);

    static void SetParameter(Chan ch, CommandPanel command, float value);

    static void SetOffset(Chan ch, float offset);

    /// Выбор источника манипуляции
    static struct SourceManipulation
    {
        enum E
        {
            None,
            COMP1,      ///< Сигнал COMP1 - микросхемы AD9952 канала A
            COMP2,      ///< Сигнал COMP2 - микросхемы AD9952 канала B
            ImpulseA,   ///< Сигнал формирователя импульсов канала A
            ImpulseB,   ///< Сигнал формирователя импульсов канала B
            Number
        } value;
        operator uint8() const { return (uint8)value; };
    } sourceManipulation[Chan::Number];

private:

    static void SetFrequency(Chan ch, float frequency);

    static void SetAmplitude(Chan ch, float amplitude);

    static void SetPhase(Chan ch, float phase);

    static void SetDutyRatio(Chan ch, float dutyRatio);

    static void SetDuration(Chan ch, float duration);

    static void SetDelay(Chan ch, float duration);
};
