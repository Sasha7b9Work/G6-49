#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Hardware
{
public:

    void Init();
    /// Установить на PA2 признак того, что процессор занят
    void SetBusy();
    /// Установить на PA2 признак того, что процессор свободен
    void SetReady();

private:

    void GPIOS_Init();

    void SystemClock_Config();
};


extern Hardware hardware;
