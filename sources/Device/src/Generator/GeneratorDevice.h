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

    static void EnableChannel(Chan::E ch, bool enable);

    static void SetFormWave(Chan::E ch, Form form);

    static void SetOffset(Chan::E ch, float offset);

    static void SetFrequency(Chan::E ch, float frequency);

    static void SetAmplitude(Chan::E ch, float amplitude);

    static void SetDuration(Chan::E ch, float duration);

    static void SetDutyRatio(Chan::E ch, float dutyRatio);

    static void SetPhase(Chan::E ch, float phase);

    static void SetDelay(Chan::E ch, float duration);

    static void SetManipulationDuration(Chan::E ch, float duration);

    static void SetManipulationPeriod(Chan::E ch, float period);

    static void SetPacketPeriod(Chan::E ch, float duration);

    static void SetPacketNumber(Chan::E ch, float number);

    static void SetPeriod(Chan::E ch, float period);
};
