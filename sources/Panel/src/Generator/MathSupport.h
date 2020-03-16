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
    } value;

    pString Suffix(uint lang) const;
};


struct MathFloatValue
{
    // Возвращает числовое представление FloatValue с numDigits значащими цифрами и запятой. В order сохраняется порядок числа относительно запятой
    static pString GetStringValue(const FloatValue &value, bool sign, int numDigits, Order::E *order, int posFirst = 255);

    // Возвращает позицию первого ненулевого символа
    // "1" - десятки (1e1), "0" - единицы (1e0), "-1" - десятые (1e-1), "-2" - сотые (1e-2)
    static int GetPositionFirstDigit(const FloatValue &value);

    // Возвращает цифру в позиции position
    static int GetDigit(const FloatValue &value, int position);

    // Возвращает символ в позиции position
    static char GetChar(const FloatValue &value, int postition);

    // Возвращает позицию запятой относительно позиции первого значащего символа
    static int PositionComma(int posFirstDigit, Order::E *order);
};


struct LogicFloatValue
{
    static void SetParameter(Parameter *param);

    // Делает попытку изменить знак и возвращает true, если это возможно
    static bool ChangedSign();

    // Изменить значение числа в позиции pos, где pos - номер десятичного разряда, начиная от 10e0
    static void ChangeDigit(int pos, int delta);

private:

    static FloatValue *value;

    static ParameterValue *parameter;
};
