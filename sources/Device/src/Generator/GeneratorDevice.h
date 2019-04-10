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

    static void SetFormWave(Chan::E ch, Form::E form);

    static void SetOffset(Chan::E ch, ParamValue offset);

    static void SetFrequency(Chan::E ch, ParamValue frequency);

    static void SetAmplitude(Chan::E ch, ParamValue amplitude);

    static void SetDuration(Chan::E ch, ParamValue duration);

    static void SetDutyRatio(Chan::E ch, ParamValue dutyRatio);

    static void SetPhase(Chan::E ch, ParamValue phase);

    static void SetDelay(Chan::E ch, ParamValue duration);

    static void SetManipulationDuration(Chan::E ch, ParamValue duration);

    static void SetManipulationPeriod(Chan::E ch, ParamValue period);

    static void SetPacketPeriod(Chan::E ch, ParamValue duration);

    static void SetPacketNumber(Chan::E ch, ParamValue number);

    static void SetPeriod(Chan::E ch, ParamValue period);
};
