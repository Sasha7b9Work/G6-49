#pragma once
#include "CommonTypes.h"


struct DFreqMeter
{
    static void Init();

    static void Update();

    static void SetResist(FreqResist resist);

    static void SetCouple(FreqCouple couple);

    static void SetFiltr(FreqFiltr filtr);
};
