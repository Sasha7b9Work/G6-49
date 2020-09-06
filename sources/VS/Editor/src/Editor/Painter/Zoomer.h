#pragma once


/*
*   Управление масштабом и отрисовка окна памяти
*/


class Zoomer
{
public:

    // Увеличить масштаб
    static void Increase(int mouseX = -1);

    // Уменьшить масштаб
    static void Decrease(int mouseX = -1);

    // Отмасштабировать по размеру выделенного региона
    static void SetOnRegion();

    static bool NoScaling();

    static int Scale();

    static void Draw();

private:

    static int scale;
};
