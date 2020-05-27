#pragma once


struct FloatValue;
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
};


struct MathFloatValue
{
    // ¬озвращает числовое представление FloatValue с numDigits значащими цифрами и зап€той.
    static pString GetIndicatedValue(const ParameterValue *param);

    // ¬озвращает позицию первого ненулевого символа "1" - дес€тки (1e1), "0" - единицы (1e0), "-1" - дес€тые (1e-1), "-2" - сотые (1e-2)
    static int GetPositionFirstDigit(const FloatValue &value);

    // ¬озвращает цифру в позиции position. “очка находитс€ соответственно order. One - после единиц, Kilo - после тыс€ч и так далее.
    // Order::Count - значенине по умолчанию - зап€та€ в позиции относительно размерности числового значени€
    static int GetDigit(const FloatValue &value, int position, Order::E order = Order::Count);

    // ¬озвращает символ в позиции position. «нак не учитываетс€. “очка находитс€ соответственно order. One - после единиц, Kilo - после тыс€ч и так далее.
    // Order::Count - значенине по умолчанию - зап€та€ в позиции относительно размерности числового значени€
    static char GetChar(const FloatValue &value, int postition, Order::E order = Order::Count);

    // ¬озвращает позицию зап€той относительно позиции первого значащего символа
    static int PositionComma(int posFirstDigit);

    // ¬озвращает:
    // Order::Mega  - FloatValue::Integer() >= 1e6
    // Order::Kilo  - FloatValue::Integer() >= 1e3
    // Order::One   - FloatValue::Integer() >= 0
    // Order::Milli - FloatValue::Integer() >= 1e-3
    // Order::Micro - FloatValue::Integer() >= 1e-6
    static Order::E GetOrder(const FloatValue value);

private:

    static void CorrectValueOnOrder(FloatValue *value, Order::E order);
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
