#pragma once
#include "Keyboard/Controls.h"


class ParameterValue;
class Tuner;
class TunerDisplay;


class Digit
{
public:

    static const char COMMA = '.';

    Digit(char v = '0') : value(v) { }

    operator char() { return value; }

    // Увеличить значение в разряде. Если невозможно (равно '9') - возращает false
    bool Increase();

    // Уменьшить значение в разряде. Если невозможно (равно '0') - возвращает false
    bool Decrease();

    // Установить новое значение
    void Set(char v) { value = v; }

    bool IsDigit()
    {
        return (value >= '0' && value <= '9');
    }

private:

    char value;
};


// Отображает знакоместа, изображения в знакоместах, и стрелки для подсвечивания активного знакоместа
class Indicator
{
public:

    Indicator(TunerDisplay *display);

    void Draw(int x, int y);

    bool OnControlKey(const Control control);

    Digit *Digits() { return digits; }

private:

    static const int MAX_NUM_DIGITS = 25;

    Digit digits[MAX_NUM_DIGITS];           // Здесь находятся все символы

    int indexHighlight;                     // Индекс подсвеченного знакоместа

    TunerDisplay *display;

    // Нарисовать значок подсветки
    void HighlightSymbol(int x, int y);

    // Возвращает индекс последнего действительного знакоместа
    int LastDigit();

    // Возвращает true, если в позиции pos - десятичная точка
    bool CommaInPosition(int pos);

    // Возвращает true, если в позиции pos - первая значащая цифра
    bool FirstSignedDigitInPosition(int pos);

    // Возвращает позицию ближайшего слева цифрового знакоместа от pos. -1, если такового не имеется
    int FindPositionLeftDigit(int pos);

    // Сделать активным разряд слева от текущего
    void HighlightToLeft();

    // Сделать аткивным разряд справа от текущего
    void HighlightToRight();

    // Увеличить значение в указанном разряде
    void IncreaseInPosition(int pos);

    // Уменьшить значение в указанном разряде
    void DecreaseInPosition(int pos);

    // Возвращает true, если значение знакоместа pos может быть уменьшено
    bool CanBeDecreased(int pos);

    // Возвращает true, если значение знакоместа pos может быть увеличено
    bool CanBeIncreased(int pos);
};


// Визуальное отображение Tuner
class TunerDisplay
{
public:
    TunerDisplay(Tuner *tuner);

    void Init();

    void Init(FloatValue value);

    void Draw();

    bool OnControlKey(const Control control);

    // Возвращает значение, закодированное в окне ввода
    FloatValue GetValue();

    Tuner *GetTuner() { return tuner; }

private:
    Tuner *tuner;

    Indicator indicator;

    void DrawTitle(int x, int y, int width);

    void DrawValue(int x, int y);
};


// Используется для визуальной настройки параметра. Является принадлежностью каждог ParameterValue
class Tuner
{

public:

    Tuner(ParameterValue *param);

    void Init();

    void Draw();

    bool OnControlKey(const Control control);

    ParameterValue *GetParameter() { return param; }

private:

    ParameterValue *param;           // Настраиваемый параметр

    TunerDisplay display;
};
