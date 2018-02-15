#pragma once
#include "Keyboard/Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Функцию надо вызывать при вызове окна на экран

class InputWindow
{
public:
    static void KeyLeft();
    static void KeyRight();
    static void KeyEnter();
};

void InputWindow_Init();

void InputWindow_Draw();

void InputWindow_ProcessContorl(StructControl control);
