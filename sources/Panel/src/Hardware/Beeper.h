// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


namespace Beeper
{
    void Init();

    // Звук нажатия на кнопку.
    void ButtonPress();

    // Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    void ButtonRelease();

    void GovernorChangedValue();

    void WarnBeepBad();

    void WarnBeepGood();

    void Beep(Action::E action);

    void Test();

    uint8 Volume();
};
