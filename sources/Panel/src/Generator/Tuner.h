#pragma once


/*
    Используется для настройки параметра
*/


class Parameter;


class Tuner
{
public:

    Tuner(Parameter *param);

private:

    Parameter *param;           // Настраиваемый параметр


};
