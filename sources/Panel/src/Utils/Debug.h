#pragma once


// Хорошо использовать для трассировке, если потом в HardFault_Handler() отслеживать эти переменные
//#define DEBUG_POINT Debug::line = __LINE__; Debug::file = __FILE__;
//#define DEBUG_POINT


namespace Debug
{
    void StartProfiling();
    void PointProfiling(char *name);

    void ClearTimeCounter();
    void StartIncreaseCounter();
    void StopIncreaseCounter();
    uint GetTimeCounterUS();

    bool ShowSends();

    extern int line;
    extern char *file;

    /// Возвращает размер свободной памяти в куче
    uint FreeHeap();

    void LogBuffer16(uint16 *buffer);

    void LogBufferF(const float *buffer);
};
