// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "Signals/Wave.h"


// Текущий сигнал
#define WAVE(ch)            (waves[ch.value])
#define CURRENT_WAVE        (WAVE(CURRENT_CHANNEL))
#define WAVE_B              WAVE((int)ChB)

#define FORM(ch)            (WAVE(ch).GetCurrentForm())
#define CURRENT_FORM        (FORM(CURRENT_CHANNEL))
#define FORM_A              FORM(ChA)
#define FORM_B              FORM((int)ChB)

#define PARAM(form)         ((form)->CurrentParameter())
#define CURRENT_PARAM       (PARAM(CURRENT_FORM))

extern Wave waves[Chan::Count];


namespace A
{
    namespace Sine
    {
        extern Form *form;
    }

    namespace RampPlus
    {
        extern Form *form;
    }

    namespace RampMinus
    {
        extern Form *form;
    }

    namespace Triangle
    {
        extern Form *form;
    }

    namespace Meander
    {
        extern Form *form;
    }

    namespace Impulse
    {
        extern Form           *form;
        extern PModeStartStop *start_stop;
        extern PDelay         *delay;
        extern PModeStart     *mode_start;
        extern PModeStartStop *mode_start_stop;
        extern PPeriod        *period;
    }

    namespace Packet
    {
        extern Form          *form;
        extern PPeriodPacket *period_packet;
        extern PPeriod       *period_impulse;
        extern PDuration     *duration;
        extern IParam        *number;
    }

    namespace Free
    {
        extern Form *form;
    }
};


namespace B
{
    namespace Sine
    {
        extern Form *form;
    }

    namespace RampPlus
    {
        extern Form *form;
    }

    namespace RampMinus
    {
        extern Form *form;
    }

    namespace Triangle
    {
        extern Form *form;
    }

    namespace Meander
    {
        extern Form *form;
    }

    namespace Impulse
    {
        extern Form       *form;
        extern PPeriod    *period;
        extern PDuration  *duration;
        extern PModeStart *mode_start;
    }

    namespace Free
    {
        extern Form *form;
    }
};
