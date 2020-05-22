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


struct FloatValue
{
    explicit FloatValue(int units, uint mUnits, uint uUnits, uint nUnits = 0);

    explicit FloatValue(float v);

    // Берёт значение из строкового представления. При этом первым символом может идти знак ("+" или "-"), дробная часть отделяется от целой точкой ("."),
    // а order указыват, на сколько нужно умножжить итоговое число (3 - умножить на 1000, -3 - разделить на 1000)
    //explicit FloatValue(const char *const buffer, int order);

    void Construct(int units, uint mUnits, uint uUnits = 0, uint nUnits = 0);

    void FromFloat(float v);

    void FromUINT64(uint64 v) { value = v; }

    void FromString(const char * const buffer, int order);

    float ToFloat() const;

    uint64 ToUINT64() const { return value; }

    int Sign() const;

    uint64 Abs() const;

    void Divide(uint div);

    void Multiplie(uint mul);
    
    // Увеличить на заданное значение
    void Add(float v);

    void Add(FloatValue value);

    void SetSign(int sign);
    
    // Возвращает целую часть числа
    int Integer() const;
    
    // Возвращает дробную часть числа. numDigits - требуемое количество знакомест после запятой
    int Fract(int numDigits) const;
    
    // Возвращает количество наночастиц в дробной части
    int FractNano() const;

    bool operator<(const FloatValue &);

    bool operator>(const FloatValue &);

private:

    uint64 value;       // Значение параметра в единицах измерения "нано". Установленный в "1" старший бит означает, что число отрицательное
};
