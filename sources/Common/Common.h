#pragma once


struct TypeForm
{
    enum E
    {
        Sine,           // Синус
        RampPlus,       // Пила+
        RampMinus,      // Пила-
        Triangle,       // Треугольник
        Meander,        // Меандр
        Impulse,        // Импульсы
        PacketImpuls,   // Пачка импульсов
        Free,           // Произвольный сигнал
        Count
    };
};


struct DoubleValue
{
    explicit DoubleValue(double v);

    // Берёт значение из строкового представления. При этом первым символом может идти знак ("+" или "-"), дробная часть отделяется от целой точкой ("."),
    // а order указыват, на сколько нужно умножжить итоговое число (3 - умножить на 1000, -3 - разделить на 1000)
    explicit DoubleValue(const char *const buffer, int order = 0);

    void FromUnits(int units, uint mUnits, uint uUnits, uint nUnits, int sign);
    void FromDouble(double v);
    void FromUINT64(uint64 v) { value = v; }
    void FromString(const char * const buffer, int order);

    double ToDouble() const;
    uint64 ToUINT64() const { return value; }

    void Div(uint div);
    void Mul(uint mul);
    void Add(DoubleValue value);
    void Sub(DoubleValue value);
    // Умножить на 10 в степени pow
    void MulPow10(int pow);

    void SetSign(int sign);

    // Возвращает знак
    int Sign() const;

    // Возвращает целую часть
    int Integer() const;
      
    // Возвращает количество наночастиц в дробной части
    int FractNano() const;

    uint64 Abs() const;

    bool operator<(const DoubleValue &);
    bool operator>(const DoubleValue &);
    bool operator<=(const DoubleValue &);
    bool operator>=(const DoubleValue &);
    bool operator==(const DoubleValue &);

private:

    uint64 value;       // Значение параметра в единицах измерения "нано". Установленный в "1" старший бит означает, что число отрицательное
};
