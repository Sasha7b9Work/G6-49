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

    void DrawDoubleValue(int x, int y);

    void DrawChoiceValue(int x, int y);

    void DrawCompositeValue(int x, int y);

    void DrawEmptyValue(int x, int y);

    Parameter *param;
};
