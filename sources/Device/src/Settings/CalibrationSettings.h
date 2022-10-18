// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Command.h"
#include "common/CommonTypes.h"
#include "Generator/Calibrator.h"


#ifndef WIN32
#pragma anon_unions
#endif


struct CalibrationSettings
{
#define NUM_CHAN       2

#define NUM_SIGNALS    2

#define NUM_RANGES     5

#define NUM_PARAMETERS 4
    /*
    * 0 : размах
    * 1 : +5В
    * 2 : 0В
    * 3 : -5В
    */

    union //-V2514
    {
        struct
        {
            int16 cal_a_imp[NUM_CHAN];                                      // Калибровка для импульсов

            int16 cal[NUM_CHAN][NUM_SIGNALS][NUM_RANGES][NUM_PARAMETERS];

            int16 kFreqMeterTrig;       // Калибровочный коэффициент уровня синхронизации частотомера
        };

        uint8 unused[256];
    };
    
    CalibrationSettings& operator =(const CalibrationSettings &);

    static int16 *GetK(uint8 channel, CalSignal::E, uint8 range, uint8 parameter);

    static int16 *GetFreqMeterK();
};


extern CalibrationSettings setCal;
