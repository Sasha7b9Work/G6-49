#pragma once
#include "defines.h"
#include "AD9952.h"
#include "AD5697.h"
#include "FPGA.h"
#include "Command.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Generator
{
    static const int DDS_NUM_POINTS = 8 * 1024;

    void Init();

    void EnableChannel(Chan::E ch, bool enable);

    void SetFormWave(Chan::E ch, Form::E form);

    void SetOffset(Chan::E ch, ParamValue offset);

    void SetFrequency(Chan::E ch, ParamValue frequency);

    void SetAmplitude(Chan::E ch, ParamValue amplitude);

    void SetDuration(Chan::E ch, ParamValue duration);

    void SetDutyRatio(Chan::E ch, ParamValue dutyRatio);

    void SetPhase(Chan::E ch, ParamValue phase);

    void SetDelay(Chan::E ch, ParamValue duration);

    void SetManipulationDuration(Chan::E ch, ParamValue duration);

    void SetManipulationPeriod(Chan::E ch, ParamValue period);

    void SetPacketPeriod(Chan::E ch, ParamValue duration);

    void SetPacketNumber(Chan::E ch, ParamValue number);

    void SetPeriod(Chan::E ch, ParamValue period);
};
