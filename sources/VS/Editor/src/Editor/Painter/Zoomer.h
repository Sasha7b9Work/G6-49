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

    // Координата y отрисовки полосы просмотра
    static int Y();

    // Ширина полосы просмотра
    static int Width();

    // Высота полосы просмотра
    static int Height();

    // Индекс первой выводимой точки
    static int IndexFirsPoint();

    // Количество выводимых точек
    static int NumberDrawingPoints();

private:

    static int scale;

    static int indexFirstPoint;

    static int indexMiddlePoint;

    static int numberDrawingPoints;

    // Нарисовать окно содержимого, отображаемого на экране
    static void DrawWindow();

    // Перерасчёт индексов ключевых точек
    static void CalculateIndexesPoints();
};
