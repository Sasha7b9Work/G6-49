#pragma once
#include "defines.h"
#include "Settings/Settings.h"


class Wave
{
public:
    pString Name(uint num, Language lang);
    /// Возвращает текущую форму сигнала на данном канале
    //static WaveForm GetForm(Chan ch = (Chan::E)CURRENT_CHANNEL);
    /// Устанавливает сигнал текущим на данный канал
    static void SetForm(Chan ch, WaveForm form);
};
