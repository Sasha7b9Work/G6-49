#pragma once
#include "Keyboard/Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Окно редактирования численного значения параметра
class InputWindow
{
public:
    static void Init();
    static void Draw();
    static void KeyLeft();
    static void KeyRight();
    static void KeyEnter();
    static void ProcessContorl(StructControl control);
};
