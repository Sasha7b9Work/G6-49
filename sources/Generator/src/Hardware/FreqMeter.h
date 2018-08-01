#pragma once
#include "CommonTypes.h"


class FreqMeter
{
public:
    static void Init();

    static void SetResist(FreqResist resist);

    static void SetCouple(FreqCouple couple);

    static void SetFiltr(FreqFiltr filtr);
};
