// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/CommonTypes.h"


namespace DFreqMeter
{
    void Init();

    void Update();

    void SetResist(FreqResist resist);

    void SetCouple(FreqCouple couple);

    void SetFiltr(FreqFiltr filtr);
};
