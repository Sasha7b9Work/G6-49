#pragma once


/*
    Визуализатор значения параметра
*/


class Parameter;


class Viewer
{
public:

    Viewer(Parameter *param);

    // Отрисовать значение в "закрытом" виде
    void Draw(int x, int y);

    // Отрисовать значение в режиме настройки
    void DrawTuned();

private:

    void DrawParameterValue(int x, int y);

    Parameter *param;
};
