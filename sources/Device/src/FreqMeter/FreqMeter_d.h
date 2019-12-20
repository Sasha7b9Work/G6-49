#pragma once
#include "CommonTypes.h"


namespace FreqMeter
{
    void Init();

    void Update();

    void SetResist(FreqResist resist);

    void SetCouple(FreqCouple couple);

    void SetFiltr(FreqFiltr filtr);
};
