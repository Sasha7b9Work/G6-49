#pragma once
#include "Settings/Settings.h"


struct Beeper
{
    static void Init();

    static void Beep(Action::E action);
};
