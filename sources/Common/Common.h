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


struct Order
{
    enum E
    {
        Mega,
        Kilo,
        One,
        Milli,
        Micro,
        Nano,
        Count
    } value;

    //pString Suffix(Language::E lang) const;
};



struct FloatValue
{
    explicit FloatValue(int units, uint mUnits, uint uUnits = 0, uint nUnits = 0);

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
    /// Возвращает количество наночастиц в дробной части
    int FractNano() const;

    struct Math
    {
        static pString GetStringDigits(const FloatValue &value, int numDigits, Order::E *order);
        /// Возвращает позицию первого ненулевого символа
        /// "1" - десятки (1e1), "0" - единицы (1e0), "-1" - десятые (1e-1), "-2" - сотые (1e-2)
        static int GetPositionFirstDigit(const FloatValue &value);
        /// Возвращает цифру в позиции position
        static int GetDigit(const FloatValue &value, int position);

        static int PositionComma(int posFirstDigit, Order::E *order);
    };

private:

    uint64 value;       // Значение параметра в единицах измерения "нано". Установленный в "1" старший бит означает, что число отрицательное
};
