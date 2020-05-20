#pragma once


/*
    Используется для настройки параметра
*/


class Parameter;
struct Key;


class Tuner
{
public:

    Tuner(Parameter *param);

    void Draw(int x, int y);

    void OnKeyControl(const Key &control);

private:

    Parameter *param;           // Настраиваемый параметр
};
