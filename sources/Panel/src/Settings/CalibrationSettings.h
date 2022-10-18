// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Command.h"
#include "common/CommonTypes.h"
#include "Menu/Pages/Pages.h"
#include "common/Messages.h"


#ifndef WIN32

#pragma anon_unions

#else

#pragma warning(push)
#pragma warning(disable:4201)   // nonstandard extension used: nameless struct/union

#endif

struct CalibrationSettings
{
#define NUM_CHAN       2

#define NUM_SIGNALS    2

#define NUM_RANGES     5
    /*
    * 0 : 100 mV
    * 1 : 300 mV
    * 2 : 1 V
    * 3 : 3 V
    * 4 : 10 V
    */

#define NUM_PARAMETERS 4
    /*
    * 0 : ������
    * 1 : +5�
    * 2 : 0�
    * 3 : -5�
    */

    union //-V2514
    {
        struct
        {
            int16 cal_amp_imp[NUM_CHAN];                                    // ������������� ����������� ��� ���������� ���������
            int16 cal[NUM_CHAN][NUM_SIGNALS][NUM_RANGES][NUM_PARAMETERS];
            int16 kFreqMeter;                                               // ������������� ����������� ��� ������ ������������� ����������
            uint8 soundVolume;

        };
        uint8 unused[256];
    };

    void Save();
    void Load();

    CalibrationSettings &operator =(const CalibrationSettings &);

    static int16 *GetK(uint8 channel, uint8 signal, uint8 range, uint8 parameter);

    // ��������� ������� �
    void ReduceK(int16 min);

    // ��������� ������� �
    void IncreaseK(int16 max);
};


extern CalibrationSettings setCal;


#ifdef WIN32
#pragma warning(pop)
#endif
