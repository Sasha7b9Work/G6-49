#pragma once
#include "defines.h"


struct CPU
{
    static void Init();
    /// Установить на PA2 признак того, что процессор занят
    static void SetBusy();
    /// Установить на PA2 признак того, что процессор свободен
    static void SetReady();

private:

    static void InitGPIOS();
};
