#pragma once


/*
    Визуализатор значения параметра
*/


class Parameter;


class Viewer
{
public:

    Viewer(Parameter *param);

    // Отрисовать параметр со значением в горизонтали y. Название параметра выводится с xName, значение - с xValue, единциы измерения - c xUnits
    void Draw(int y, int xName, int xValue, int xUnits, const Chan &);

    void DrawName(int x, int y, int width, const Chan &);

private:

    void DrawValue(int y, int xValue, int xUnits);

    void DrawChoice(int y, int xValue, int xUnits);

    void DrawComposite(int y, int xValue, int xUnits);

    void DrawButton(int y, int xValue, int xUnits);

    Parameter *param;
};
