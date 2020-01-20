#pragma once
#include "common/Command.h"
#include "common/CommonTypes.h"
#include "Menu/Pages/PageSignals/PageSignals.h"
#include "common/Message.h"



#define CAL_AD9952_OFFSET_NEG(ch)   setCal.cal_AD9952_Offset_Negative[ch]
#define CAL_AD9952_OFFSET_POS(ch)   setCal.cal_AD9952_Offset_Positive[ch]
#define CAL_AD9952_OFFSET_ZERO(ch)  setCal.cal_AD9952_Offset_Zero[ch]
#define CAL_AD9952_AMPLITUDE(ch)    setCal.cal_AD9952_Amplitude[ch]
#define CAL_DDS_MAX(ch)             setCal.cal_DDS_MAX[ch]
#define CAL_DDS_MIN(ch)             setCal.cal_DDS_MIN[ch]
#define CAL_DDS_OFFSET(ch)          setCal.cal_DDS_Offset[ch]
#define CAL_FREQ_LEVEL_TRIG         setCal.cal_FREQ_Level_Trig




#ifndef WIN32

#pragma anon_unions

#else

#pragma warning(push)
#pragma warning(disable:4201)   // nonstandard extension used: nameless struct/union

#endif

struct CalibrationSettings
{
    union //-V2514
    {
        struct
        {
            const   uint marker;                                ///< По этому значению загрузчик настроек определяет, что они есть
            int16   cal_AD9952_Offset_Negative[Chan::Count];   ///< Калибровочный коэффициент AD9952 на -5В
            int16   cal_AD9952_Offset_Zero[Chan::Count];       ///< Калибровочный коэффициент AD9952 на 0В
            int16   cal_AD9952_Offset_Positive[Chan::Count];   ///< Калибровочный коэффициент AD9952 на +5В
            int16   cal_AD9952_Amplitude[Chan::Count];         ///< Калибровочный коэффициент AD9952 размаха
            int16   cal_DDS_MAX[Chan::Count];
            int16   cal_DDS_MIN[Chan::Count];
            int16   cal_DDS_Offset[Chan::Count];
            int16   cal_FREQ_Level_Trig;                        ///< Калибровочный коэффициенрт уровня синхронизации
            uint8   nu[2];

#define NUM_CHAN       2
#define NUM_SIGNALS    2
#define NUM_RANGES     6
#define NUM_PARAMETERS 4
            int16 cal[NUM_CHAN][NUM_SIGNALS][NUM_RANGES][NUM_PARAMETERS];
        };
        uint8 unused[512];
    };
    
    void Save();
    void Load();

    CalibrationSettings& operator =(const CalibrationSettings &);

    static int16 *GetK(uint8 channel, uint8 signal, uint8 range, uint8 parameter);
    /// Уменьшить текущий к
    void ReduceK();
    /// Увеличить текущий к
    void IncreaseK();
};


extern CalibrationSettings setCal;


#ifdef WIN32
#pragma warning(pop)
#endif
