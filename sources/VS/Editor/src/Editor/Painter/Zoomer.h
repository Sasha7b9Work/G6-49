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

    // Координата x отрисовки полосы просмотра
    static int X();

    static int Right();

    // Координата y отрисовки полосы просмотра
    static int Y();

    // Ширина полосы просмотра
    static int Width();

    // Высота полосы просмотра
    static int Height();

    static int Bottom();

    // Индекс первой выводимой точки
    static int IndexFirsPoint();

    // Количество выводимых точек
    static int NumberDrawingPoints();

    // Возвращает true, если находится под мыщью
    static bool UnderMouse(int mouseX, int mouseY);

private:

    static int scale;

    static int indexFirstPoint;

    static int indexMiddlePoint;

    static int numberDrawingPoints;

    // Перерасчёт индексов ключевых точек
    static void CalculateIndexesPoints();
};
