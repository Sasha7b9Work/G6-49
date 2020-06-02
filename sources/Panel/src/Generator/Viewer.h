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

    void DrawDouble(int x, int y);

    void DrawChoice(int x, int y);

    void DrawEmpty(int x, int y);

    Parameter *param;
};
