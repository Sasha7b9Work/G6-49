#pragma once
#include "CommonTypes.h"


namespace DFreqMeter
{
    void Init();

    void Update();

    void SetResist(FreqResist resist);

    void SetCouple(FreqCouple couple);

    void SetFiltr(FreqFiltr filtr);
};
