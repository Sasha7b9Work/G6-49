#pragma once


struct DoubleValue;
class Parameter;
class ParameterValue;


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
    };

    static pString Suffix(Order::E order, uint lang);

    // ¬озвращает степень, соответсвующую пор€дку
    static int GetPow10(Order::E order);
};


struct MathFloatValue
{
    // ¬озвращает числовое представление DoubleValue с numDigits значащими цифрами и зап€той.
    static pString GetIndicatedValue(const ParameterValue *param);

    // ¬озвращает позицию первого ненулевого символа "1" - дес€тки (1e1), "0" - единицы (1e0), "-1" - дес€тые (1e-1), "-2" - сотые (1e-2)
    static int GetPositionFirstDigit(const ParameterValue *param, Order::E order = Order::Count);

    // ¬озвращает цифру в позиции position. “очка находитс€ соответственно order. One - после единиц, Kilo - после тыс€ч и так далее.
    // Order::Count - значенине по умолчанию - зап€та€ в позиции относительно размерности числового значени€
    static int GetDigit(const DoubleValue &value, int position, Order::E order = Order::Count);

    // ¬озвращает символ в позиции position. «нак не учитываетс€. “очка находитс€ соответственно order. One - после единиц, Kilo - после тыс€ч и так далее.
    // Order::Count - значенине по умолчанию - зап€та€ в позиции относительно размерности числового значени€
    static char GetChar(const DoubleValue &value, int postition, Order::E order = Order::Count);

    // ¬озвращает:
    // Order::Mega  - DoubleValue::Integer() >= 1e6
    // Order::Kilo  - DoubleValue::Integer() >= 1e3
    // Order::One   - DoubleValue::Integer() >= 0
    // Order::Milli - DoubleValue::Integer() >= 1e-3
    // Order::Micro - DoubleValue::Integer() >= 1e-6
    static Order::E GetOrder(DoubleValue value);

private:

    static void CorrectValueOnOrder(DoubleValue *value, Order::E order);
};


struct MathParameterValue
{
    static void SetParameterValue(ParameterValue *parameter) { param = parameter; };

    // ¬озвращает количество значащих знакомест перед зап€той (исключа€ знаковый разр€д)
    static int GetNumberDigitsBeforeComma();

    // ¬озвращает количество значащих знакомест после зап€той
    static int GetNumberDigitsAfterComma();

private:

    // —обственно параметр, по которому будут производитс€ вычислеин€
    static ParameterValue *param;
};
