#pragma once
#include "Keyboard/Controls.h"


/*
    Используется для настройки параметра
*/


class Parameter;


class Tuner
{
public:

    Tuner(Parameter *param);

    void Draw();

    bool OnKeyControl(const Control control);

private:

    Parameter *param;           // Настраиваемый параметр
};
