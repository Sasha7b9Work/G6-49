#pragma once
#include "CommonTypes.h"


struct Amplifier
{
    static void Init();
    static void Tune(Chan::E ch);
private:
    static void SetState(Chan::E ch, bool state);
};
