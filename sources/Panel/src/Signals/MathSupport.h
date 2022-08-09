// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"


struct Value;
struct DParam;
struct IParam;


namespace MathDouble
{
    // ¬озвращает числовое представление Value с numDigits значащими цифрами и зап€той.
    cstr GetIndicatedValue(const DParam *param);

    // ¬озвращает позицию первого ненулевого символа "1" - дес€тки (1e1), "0" - единицы (1e0), "-1" - дес€тые (1e-1), "-2" - сотые (1e-2)
    int GetPositionFirstDigit(const DParam *param, Order::E order = Order::Count);

    int GetPositionFirstDigit(const Value &value, Order::E order);

    // ¬озвращает цифру в позиции position. “очка находитс€ соответственно order. One - после единиц, Kilo - после тыс€ч и так далее.
    // Order::Count - значенине по умолчанию - зап€та€ в позиции относительно размерности числового значени€
    int GetDigit(const Value &value, int position, Order::E order = Order::Count);

    // ¬озвращает символ в позиции position. «нак не учитываетс€. “очка находитс€ соответственно order. One - после единиц, Kilo - после тыс€ч и так далее.
    // Order::Count - значенине по умолчанию - зап€та€ в позиции относительно размерности числового значени€
    char GetChar(const Value &value, int postition, Order::E order);
};


namespace MathParameterDouble
{
    void SetParameterDouble(DParam *);

    // ¬озвращает количество значащих знакомест перед зап€той (исключа€ знаковый разр€д). ¬ предположении, что значение приведено к пор€дку order
    int GetNumberDigitsBeforeComma(Order::E order = Order::Count);

    // ¬озвращает количество значащих знакомест после зап€той
    int GetNumberDigitsAfterComma(Order::E order = Order::Count);
};


namespace MathParameterInteger
{
    // ¬озвращает количество разр€дов дл€ параметра parameter (в них должно помещатьс€ максимально возможное значение параметра parameter)
    int GetMaxNumberDigits(const IParam *parameter);
};
