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

    static void EnableChannel(Channel ch, bool enable);

    static void SetFormWave(Channel ch, Type_WaveForm form);

    static void SetParameter(Channel ch, CommandWrite command, float value);

    static void SetOffset(Channel ch, float offset);

private:

    static void SetFrequency(Channel ch, float frequency);

    static void SetAmplitude(Channel ch, float amplitude);

    //void SetOffset(Channel ch, float offset);

    static void SetPhase(Channel ch, float phase);

    static void SetDutyRatio(Channel ch, float dutyRatio);

    static void SetDuration(Channel ch, float duration);

    static void SetDelay(Channel ch, float duration);
};
