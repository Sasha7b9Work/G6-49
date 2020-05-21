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
    Digit(char v = ' ') : value(v) { }
private:
    char value;
};


class TunerDisplay
{
public:
    TunerDisplay(ParameterValue *parameter) : param(parameter) { }

    void Draw();

private:
    ParameterValue *param;

    Title title;

    static const int MAX_NUM_DIGITS = 15;

    // Здесь находятся все символы
    Digit digits[MAX_NUM_DIGITS];

    void DrawTitle(int x, int y, int width);

    void DrawValue(int x, int y);
};


class Tuner
{
public:

    Tuner(ParameterValue *param);

    void Draw();

    bool OnKeyControl(const Control control);

private:

    ParameterValue *param;           // Настраиваемый параметр

    TunerDisplay display;
};
