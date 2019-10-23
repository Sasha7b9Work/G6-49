#pragma once
#include "defines.h"



// Хорошо использовать для трассировке, если потом в HardFault_Handler() отслеживать эти переменные
//#define DEBUG_POINT Debug::line = __LINE__; Debug::file = __FILE__;



namespace Debug
{
    void StartProfiling();
    void PointProfiling(char *name);

    void ClearTimeCounter();
    void StartIncreaseCounter();
    void StopIncreaseCounter();
    uint GetTimeCounterUS();

    extern int line;
    extern char *file;
};
