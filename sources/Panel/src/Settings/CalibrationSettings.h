#pragma once
#include "Command.h"
#include "CommonTypes.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CAL_AD9952_OFFSET_NEG(ch)   setCal.cal_AD9952_Offset_Negative[ch]
#define CAL_AD9952_OFFSET_POS(ch)   setCal.cal_AD9952_Offset_Positive[ch]
#define CAL_AD9952_OFFSET_ZERO(ch)  setCal.cal_AD9952_Offset_Zero[ch]
#define CAL_AD9952_AMPLITUDE(ch)    setCal.cal_AD9952_Amplitude[ch]
#define CAL_DDS_MAX(ch)             setCal.cal_DDS_MAX[ch]
#define CAL_DDS_MIN(ch)             setCal.cal_DDS_MIN[ch]


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef WIN32
#pragma anon_unions
#endif

struct CalibrationSettings
{
    union
    {
        struct
        {
            const   uint marker;                                ///< По этому значению загрузчик настроек определяет, что они есть
            int16   cal_AD9952_Offset_Negative[Chan::Number];   ///< Калибровочный коэффициент AD9952 на -5В
            int16   cal_AD9952_Offset_Zero[Chan::Number];       ///< Калибровочный коэффициент AD9952 на 0В
            int16   cal_AD9952_Offset_Positive[Chan::Number];   ///< Калибровочный коэффициент AD9952 на +5В
            int16   cal_AD9952_Amplitude[Chan::Number];         ///< Калибровочный коэффициент AD9952 размаха
            int16   cal_DDS_MAX[Chan::Number];
            int16   cal_DDS_MIN[Chan::Number];
        };
        uint8 unused[512];
    };
    
    void Save();
    void Load();

    CalibrationSettings& operator =(const CalibrationSettings &);
};


extern CalibrationSettings setCal;
