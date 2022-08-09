// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "Signals/Wave.h"


// Текущий сигнал
#define WAVE(ch)            (waves[ch.value])
#define CURRENT_WAVE        (WAVE(CURRENT_CHANNEL))
#define WAVE_B              WAVE(ChB)

#define FORM(ch)            (WAVE(ch).GetCurrentForm())
#define CURRENT_FORM        (FORM(CURRENT_CHANNEL))
#define FORM_A              FORM(ChA)
#define FORM_B              FORM(ChB)

#define PARAM(form)         ((form)->CurrentParameter())
#define CURRENT_PARAM       (PARAM(CURRENT_FORM))

extern Wave waves[Chan::Count];


namespace A
{
    namespace Impulse
    {
        extern Form *form;
        extern PModeStartStop *start_stop;
        extern ParamDelay         *delay;
        extern PModeStart     *mode_start;
        extern ParamPeriod        *period;
    }

    namespace Packet
    {
        extern Form *form;
        extern ParamPacketPeriod *period_packet;
        extern ParamPeriod       *period_impulse;
        extern ParamDuration     *duration;
        extern IParam      *number;
    }
};


namespace B
{
    namespace Impulse
    {
        extern Form *form;
        extern ParamPeriod    *period;
        extern ParamDuration  *duration;
        extern PModeStart *mode_start;
    }
};
