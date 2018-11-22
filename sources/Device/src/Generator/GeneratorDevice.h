#pragma once
#include "defines.h"
#include "AD9952.h"
#include "AD5697.h"
#include "FPGA.h"
#include "Command.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Generator
{
public:
    static void Init();

    static void EnableChannel(Chan ch, bool enable);

    static void SetFormWave(Chan ch, Form form);

    static void SetParameter(Chan ch, Command command, float value);

    static void SetOffset(Chan ch, float offset);

    static void SetFrequency(Chan ch, float frequency);

    static void SetAmplitude(Chan ch, float amplitude);

    static void SetDuration(Chan ch, float duration);

    static void SetDutyRatio(Chan ch, float dutyRatio);

    static void SetPhase(Chan ch, float phase);

private:

    static void SetPeriod(Chan ch, float period);

    static void SetDelay(Chan ch, float duration);

    static void SetPacketPeriod(Chan ch, float duration);

    static void SetPacketNumber(Chan ch, float number);

    static void SetManipulationPeriod(Chan ch, float period);

    static void SetManipulationDuration(Chan ch, float duration);

    static void EmptyFunc(Chan, float);
};
