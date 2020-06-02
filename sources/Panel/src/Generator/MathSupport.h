#pragma once
#include "common/Common.h"


struct Value;
class Parameter;
class ParameterDouble;


struct MathValue
{
    // ¬озвращает числовое представление Value с numDigits значащими цифрами и зап€той.
    static pString GetIndicatedValue(const ParameterDouble *param);

    // ¬озвращает позицию первого ненулевого символа "1" - дес€тки (1e1), "0" - единицы (1e0), "-1" - дес€тые (1e-1), "-2" - сотые (1e-2)
    static int GetPositionFirstDigit(const ParameterDouble *param, Order::E order = Order::Count);

    static int GetPositionFirstDigit(const Value &value, Order::E order);

    // ¬озвращает цифру в позиции position. “очка находитс€ соответственно order. One - после единиц, Kilo - после тыс€ч и так далее.
    // Order::Count - значенине по умолчанию - зап€та€ в позиции относительно размерности числового значени€
    static int GetDigit(const Value &value, int position, Order::E order = Order::Count);

    // ¬озвращает символ в позиции position. «нак не учитываетс€. “очка находитс€ соответственно order. One - после единиц, Kilo - после тыс€ч и так далее.
    // Order::Count - значенине по умолчанию - зап€та€ в позиции относительно размерности числового значени€
    static char GetChar(const Value &value, int postition, Order::E order = Order::Count);

private:

    static void CorrectValueOnOrder(Value *value, Order::E order);
};


struct MathParameterValue
{
    static void SetParameterValue(ParameterDouble *parameter) { param = parameter; };

    // ¬озвращает количество значащих знакомест перед зап€той (исключа€ знаковый разр€д). ¬ предположении, что значение приведено к пор€дку order
    static int GetNumberDigitsBeforeComma(Order::E order = Order::Count);

    // ¬озвращает количество значащих знакомест после зап€той
    static int GetNumberDigitsAfterComma(Order::E order = Order::Count);

private:

    // —обственно параметр, по которому будут производитс€ вычислеин€
    static ParameterDouble *param;

    // ¬озвращает количество разр€дов перед зап€той у параметра времени
    static int GetNumberDigitsBeforeCommaTime(Order::E order = Order::Count);

    // ¬озвращает количество разр€дов перед зап€той у параметра частоты
    static int GetNumberDigitsBeforeCommaFrequency(Order::E order = Order::Count);
};
