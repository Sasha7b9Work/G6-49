#pragma once
#include "Display/Colors.h"
#include "Keyboard/Controls.h"


class ParameterValue;
class Tuner;
class DisplayCorrection;


class Digit
{
public:

    static const char COMMA = '.';

    Digit(char v = '0') : value(v) { }

    operator char() const { return value; }

    // Установить новое значение
    void Set(char v) { value = v; }

    bool IsNumber()  { return (value >= '0' && value <= '9'); }

    bool IsEmpty()   { return (value == '\0'); }

    bool IsSigned()  { return (value == '-') || (value == '+'); }

private:

    char value;
};


// Отображает знакоместа, изображения в знакоместах, и стрелки для подсвечивания активного знакоместа
class Indicator
{
    friend class DisplayCorrection;

public:

    Indicator(DisplayCorrection *display);

    int Draw(int x, int y);

    bool OnControlKey(const Control control);

    // Возвращает строку символов
    char *GetStringDigits() const;

    void InitHighlight();

    static const int MAX_NUM_DIGITS = 25;

private:

    Digit digits[MAX_NUM_DIGITS];           // Здесь находятся все символы

    int indexHighlight;                     // Индекс подсвеченного знакоместа. Счёт ведётся только по цифровым и знаковым разрядам

    DisplayCorrection *display;

    // Нарисовать значок подсветки
    void HighlightSymbol(int x, int y);

    // Возвращает действительный индекс подсвеченного разряда (номер разряда в массиве digits)
    int IndexHighlightReal();

    // Количество символов, которые могут быть подсвечены
    int NumberHighligthingDigits();

    // Возвращает true, если в позиции pos - десятичная точка
    bool CommaInPosition(int pos);

    // Возвращает позицию разряда с точкой
    int PositionComma();

    // Возвращает true, если в позиции pos - первая значащая цифра
    bool FirstSignedDigitInPosition(int pos);

    // Возвращает позицию ближайшего слева цифрового знакоместа от pos. -1, если такового не имеется.
    int FindPositionLeftDigit(int pos);

    // Сделать активным разряд слева от текущего
    void HighlightToLeft();

    // Сделать аткивным разряд справа от текущего
    void HighlightToRight();

    // Увеличить значение в указанном разряде
    void IncreaseInPosition(int pos);

    // Уменьшить значение в указанном разряде
    void DecreaseInPosition(int pos);

    // Возвращает true, если числовое значение со знакомЫ
    bool IsSigned();

    // Возвращает "цену разрдяа" в позиции pos
    DoubleValue StepPosition(int pos);

    // Изменяет знак у параметра со знаком. Возвращает true, если pos == 0
    bool ChangeSign(DoubleValue *value, int pos);

    Color CalculateColor(int pos);

    // Возвращает true, если нужно дополнительное смещение между разрядами
    int AdditionShiftForDigit(int pos);

    // Возвращает true, если все цифровые разряды слева равны нулю
    bool AllNumbersOfLeftIsZero(int pos);
    
    // Возвращает true, если все цифровые разряды справа равны нулю
    bool AllNumberOfRightIsZero(int pos);
};


// Класс для ввода и отображения параметра
class DisplayEntering
{
public:
    
    // Обработка нажатия клавиши набора цифрового значения
    static bool OnEnteringKey(const Control &control);

    static void Draw(int x, int y, int width);

private:

    static int DrawValue(int x, int y);

    static void DrawUnits(int x, int y, int width);
};


// Визуальное отображение Tuner
class DisplayCorrection
{
    friend class Tuner;

public:
    
    enum ModeTuning
    {
        Correction,     // Режим коррекциии значения параметра (ручкой)
        Entering        // Режим ввода значения
    };

    DisplayCorrection(Tuner *tuner);

    void Init();

    void Init(DoubleValue value);

    void Draw();

    bool OnControlKey(const Control &control);

    static bool InModeCorrection() { return (mode == Correction); }

    static void SetModeEntering();

private:

    Tuner *tuner;

    Indicator indicator;

    void DrawTitle(int x, int y, int width);

    // Возвращает x зз выведенными символами
    int DrawValue(int x, int y);

    void DrawUnits(int x, int y);

    // Заполнить разряды целой части параметра
    void FillDigitsIntegerPart();

    // Заполнить разряды дробной части параметра
    void FillDigitsFractPart();

    static ModeTuning mode;                         // Текущий режим настройки
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

    static Tuner *Current() { return current; };

private:

    ParameterValue *param;           // Настраиваемый параметр

    DisplayCorrection display;

    static Tuner *current;
};
