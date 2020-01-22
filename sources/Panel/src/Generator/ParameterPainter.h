#pragma once
#include "Settings/SettingsTypes.h"


struct FloatValue;
class Parameter;


class ParameterPainter
{
public:
    static void SetPatameter(Parameter *param)
    {
        parameter = param;
    }

    /// Возвращает строку значения параметра со знаком для редактитрования
    static pString DigitsWithSign();
    /// Возвращает строку с единицами измерения
    static pString Units();

private:
    static Parameter *parameter;
    /// Возвращает строку значения смещения
    static pString DigitsWithSignOffset();
    /// Возвращает строку значения параметра ParameterValue
    static pString DigitsWithSignValue();
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

    pString Suffix(Language::E lang) const;
};


struct MathFloatValue
{
    /// Возвращает числовое представление FloatValue с numDigits значащими цифрами и запятой. В order сохраняется порядок числа относительно запятой
    static pString GetStringValue(const FloatValue &value, bool sign, int numDigits, Order::E *order, int posFirst = 255);
    /// Возвращает позицию первого ненулевого символа
    /// "1" - десятки (1e1), "0" - единицы (1e0), "-1" - десятые (1e-1), "-2" - сотые (1e-2)
    static int GetPositionFirstDigit(const FloatValue &value);
    /// Возвращает цифру в позиции position
    static int GetDigit(const FloatValue &value, int position);
    /// Возвращает символ в позиции position
    static char GetChar(const FloatValue &value, int postition);
    /// Возвращает позицию запятой относительно позиции первого значащего символа
    static int PositionComma(int posFirstDigit, Order::E *order);
};
