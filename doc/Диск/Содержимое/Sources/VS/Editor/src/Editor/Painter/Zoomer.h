#pragma once


/*
*   Управление масштабом и отрисовка окна памяти
*/


class Zoomer
{
    friend struct Window;

public:

    // Увеличить масштаб
    static void Increase(int mouseX = -1);

    // Уменьшить масштаб
    static void Decrease(int mouseX = -1);

    // Отмасштабировать по размеру выделенного региона
    static void SetOnRegion();

    static bool NoScaling();

    static int Scale();

    static float ScaleF();

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

    // Индекс точки, следующей за последней выводимой
    static int IndexLastPoint();

    // Количество выводимых точек
    static int NumberDrawingPoints();

    // Возвращает true, если находится под мыщью
    static bool UnderMouse(int mouseX, int mouseY);

    static bool MoveWindow(int mouseX);

    // Захватить мышью
    static bool Grab(int mouseX, int mouseY);

    // Отвязать от мыши
    static bool UnGrab();

private:

    static int scale;
    static int indexFirstPoint;         // Индекс первой выводимой точки
    static int indexMiddlePoint;        // Индекс средней выводимой точки
    static int numberDrawingPoints;     // Количество выводимых точек

    // Перерасчёт индексов ключевых точек
    static void CalculateIndexesPoints();
};
