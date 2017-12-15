#pragma once
#include "defines.h"
#include "AD9952.h"
#include "AD5697.h"
#include "FPGA.h"
#include "../../Common/Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Generator
{
public:
    void Init();

    void EnableChannel(Channel ch, bool enable);

    void SetFormWave(Channel ch, WaveForm form);

    void SetParameter(Channel ch, CommandWrite command, float value);

    void SetOffset(Channel ch, float offset);

private:

    void SetFrequency(Channel ch, float frequency);

    void SetAmplitude(Channel ch, float amplitude);

    //void SetOffset(Channel ch, float offset);

    void SetPhase(Channel ch, float phase);

    void SetDutyRatio(Channel ch, float dutyRatio);

    void SetDuration(Channel ch, float duration);

    void SetDelay(Channel ch, float duration);

    AD9952 ad9952;

    AD5697 ad5697;

    FPGA   fpga;
};


extern Generator generator;
