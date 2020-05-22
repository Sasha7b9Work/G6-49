#pragma once
#include "Keyboard/Controls.h"


class ParameterValue;


class Digit
{
public:

    Digit(char v = '0') : value(v) { }

    operator char() { return value; }

    // Увеличить значение в разряде. Если невозможно (равно '9') - возращает false
    bool Increase();

    // Уменьшить значение в разряде. Если невозможно (равно '0') - возвращает false
    bool Decrease();

    // Установить новое значение
    void Set(char v) { value = v; }

private:

    char value;
};


// Отображает знакоместа, изображения в знакоместах, и стрелки для подсвечивания активного знакоместа
class Indicator
{
public:

    Indicator();

    void Draw(int x, int y);

    // Сделать активным разряд слева от активного
    void HighlightLeft();

    // Сделать активным разряд справа от активного
    void HighlightRight();

    // Увелчить значение в текущем разряде
    void HighlightIncrease();

    // Уменьшить значение в текщем разряде
    void HighlightDecrease();

private:

    static const int MAX_NUM_DIGITS = 15;

    Digit digits[MAX_NUM_DIGITS];           // Здесь находятся все символы

    int indexHighlight;                     // Индекс подсвеченного знакоместа

    // Нарисовать значок подсветки
    void HighlightSymbol(int x, int y);

    // Возвращает индекс последнего действительного знакоместа
    int LastDigit();

    // Возвращает true, если в позиции pos - десятичная точка
    bool CommaInPosition(int pos);
};


// Визуальное отображение Tuner
class TunerDisplay
{
public:
    TunerDisplay(ParameterValue *parameter) : param(parameter) { }

    void Draw();

    bool OnControlKey(const Control control);

private:
    ParameterValue *param;

    Indicator indicator;

    void DrawTitle(int x, int y, int width);

    void DrawValue(int x, int y);
};


// Используется для визуальной настройки параметра. Является принадлежностью каждог ParameterValue
class Tuner
{
public:

    Tuner(ParameterValue *param);

    void Draw();

    bool OnControlKey(const Control control);

private:

    ParameterValue *param;           // Настраиваемый параметр

    TunerDisplay display;
};
