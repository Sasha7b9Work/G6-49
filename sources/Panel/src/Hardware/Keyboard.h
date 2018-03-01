#pragma once
#include "Controls.h"


#define TIME_LONG_PRESSURE 1000


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:

    static void Init();
    /// Возвращает true, если буфер пуст
    static bool BufferIsEmpty();
    /// Возвращает следующий орган управления, если таковой имеется
    static StructControl GetNextControl();

private:
    static void Update();
};
