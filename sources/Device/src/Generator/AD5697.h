// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "common/Command.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"


namespace AD5697
{
    void Init();

    void SetOffset(const Chan &);

    void SetFreqLevel(double level);

    void SetFreqHysteresys(double hyst);

    // Включает/отключает режим калибровки
    void EnabledCalibrateMode(bool enable);
};
