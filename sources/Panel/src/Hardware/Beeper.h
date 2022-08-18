// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


namespace Beeper
{
    void Init();

    // ���� ������� �� ������.
    void ButtonPress();

    // ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    void ButtonRelease();

    void GovernorChangedValue();

    void WarnBeepBad();

    void WarnBeepGood();

    void Beep(Action::E action);

    void Test();

    uint8 Volume();
};
