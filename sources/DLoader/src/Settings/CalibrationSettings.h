// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Command.h"
#include "common/CommonTypes.h"


#ifndef WIN32
#pragma anon_unions
#endif


struct CalibrationSettings
{
#define NUM_CHAN       2
#define NUM_SIGNALS    2
#define NUM_RANGES     5
#define NUM_PARAMETERS 4

    union //-V2514
    {
        struct
        {
            int16 cal[NUM_CHAN][NUM_SIGNALS][NUM_RANGES][NUM_PARAMETERS];
        };
        uint8 unused[256];
    };
    
    CalibrationSettings& operator =(const CalibrationSettings &);

    static int16 *GetK(uint8 channel, uint8 signal, uint8 range, uint8 parameter);
};


extern CalibrationSettings setCal;
