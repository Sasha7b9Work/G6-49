#pragma once
#include "Keyboard/Controls.h"


/*
    Используется для настройки параметра
*/


class ParameterValue;


class Title
{
public:
};


class Digit
{
public:
};


class Indicator
{
public:
    Indicator(ParameterValue *parameter) : param(parameter) { }

    void Draw();

private:
    ParameterValue *param;

    Title title;

    static const int MAX_NUM_DIGITS = 15;

    Digit digits[MAX_NUM_DIGITS];

    void DrawTitle(int x, int y, int width);

    void DrawValue();
};


class Tuner
{
public:

    Tuner(ParameterValue *param);

    void Draw();

    bool OnKeyControl(const Control control);

private:

    ParameterValue *param;           // Настраиваемый параметр

    Indicator indicator;
};
