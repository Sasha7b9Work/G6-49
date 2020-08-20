#pragma once
#include "Settings/Settings.h"


struct Beeper
{
    static void Init();
    // Звук нажатия на кнопку.
    static void ButtonPress();
    // Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    static void ButtonRelease();

    static void GovernorChangedValue();
    
    static void WarnBeepBad();

    static void WarnBeepGood();

    static void Beep(Action::E action);

    static void Test();

    static uint8 Volume();
};
