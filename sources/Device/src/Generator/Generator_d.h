#pragma once
#include "defines.h"
#include "AD9952.h"
#include "AD5697.h"
#include "FPGA.h"
#include "Command.h"


struct DGenerator
{
    static const int DDS_NUM_POINTS = 8 * 1024;

    static void Init();

    static void EnableChannel(Chan::E ch, bool enable);

    static void SetFormWave(Chan::E ch, Form::E form);

    static void SetOffset(Chan::E ch, FloatValue offset);

    static void SetFrequency(Chan::E ch, FloatValue frequency);

    static void SetAmplitude(Chan::E ch, FloatValue amplitude);

    static void SetDuration(Chan::E ch, FloatValue duration);

    static void SetDutyRatio(Chan::E ch, FloatValue dutyRatio);

    static void SetPhase(Chan::E ch, FloatValue phase);

    static void SetDelay(Chan::E ch, FloatValue duration);

    static void SetManipulationDuration(Chan::E ch, FloatValue duration);

    static void SetManipulationPeriod(Chan::E ch, FloatValue period);

    static void SetPacketPeriod(Chan::E ch, FloatValue duration);

    static void SetPacketNumber(Chan::E ch, FloatValue number);

    static void SetPeriod(Chan::E ch, FloatValue period);
};
