#pragma once


class Zoomer
{
public:
    // Увеличить масштаб
    static void Increase(int mouseX = -1);

    // Уменьшить масштаб
    static void Decrease(int mouseX = -1);

    // Отмасштабировать по размеру выделенного региона
    static void SetOnRegion();
private:
};
