#pragma once


/*
    Визуализатор значения параметра
*/


class Parameter;


class Viewer
{
public:

    Viewer(Parameter *param);

    void Draw(int x, int y);

private:
    void DrawParameterValue(int x, int y);

    Parameter *param;
};
