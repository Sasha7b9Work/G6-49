// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


struct Beeper
{
    static void Init();
    // ���� ������� �� ������.
    static void ButtonPress();
    // ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    static void ButtonRelease();

    static void GovernorChangedValue();
    
    static void WarnBeepBad();

    static void WarnBeepGood();

    static void Beep(Action::E action);

    static void Test();

    static uint8 Volume();
};
