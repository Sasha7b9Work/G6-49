#pragma once
#include "defines.h"


struct TypeForm
{
    enum E
    {
        Sine,           ///< Синус
        RampPlus,       ///< Пила+
        RampMinus,      ///< Пила-
        Triangle,       ///< Треугольник
        Meander,        ///< Меандр
        Impulse,        ///< Импульсы
        PacketImpuls,   ///< Пачка импульсов
        Free,           ///< Произвольный сигнал
        Count
    };
};


struct FloatValue
{
    /// Значение, выраженное в единицах (units) и наноединицах (nanoUnits)
    explicit FloatValue(uint units, uint nanoUnits);

    explicit FloatValue(uint64 v) : value(v) {}

    explicit FloatValue(float v);

    float ToFloat() const;

    void FromFloat(float v);

    void FromUINT64(uint64 v) { value = v; }

    uint64 ToUINT64() const { return value; }

    int Sign() const;

    uint64 Abs() const;

    void Divide(uint div);

    void Multiplie(uint mul);
    /// Увеличить на заданное значение
    void Add(float v);

    void SetSign(int sign);
    /// Возвращает целую часть числа
    int Integer() const;
    /// Возвращает дробную часть числа. numDigits - требуемое количество знакомест после запятой
    int Fract(int numDigits) const;

private:

    uint64 value;       // Значение параметра в единицах измерения "нано". Установленный в "1" старший бит означает, что число отрицательное
};
