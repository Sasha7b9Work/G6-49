// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/CommonTypes.h"


struct DFreqMeter
{
    static void Init();

    static void Update();

    static void SetResist(FreqResist resist);

    static void SetCouple(FreqCouple couple);

    static void SetFiltr(FreqFiltr filtr);
};
