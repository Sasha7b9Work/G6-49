#pragma once
#include "Display/Colors.h"
#include "Display/Text.h"
#include "Keyboard/Controls.h"
#include "Utils/Stack.h"


class DisplayCorrection;
class Parameter;
class ParameterDouble;
class ParameterInteger;
class Tuner;


class Digit
{
public:

    static const char COMMA = '.';

    Digit(char v = '0') : value(v) { }

    operator char() const { return value; }

    // Установить новое значение
    void Set(char v) { value = v; }

    bool IsNumber() const  { return (value >= '0' && value <= '9'); }

    bool IsEmpty() const   { return (value == '\0'); }

    bool IsSigned() const  { return (value == '-') || (value == '+'); }

private:

    char value;
};


// Отображает знакоместа, изображения в знакоместах, и стрелки для подсвечивания активного знакоместа
class Indicator
{
    friend class DisplayCorrection;

public:

    Indicator(DisplayCorrection *display);

    int Draw(int x, int y, int width);

    bool OnControlKey(const Control control);

    // Возвращает строку символов
    char *GetStringDigits() const;

    void InitHighlight();

    static const int MAX_NUM_DIGITS = 25;

private:

    Digit digits[MAX_NUM_DIGITS];           // Здесь находятся все символы

    int indexHighlight;                     // Индекс подсвеченного знакоместа. Счёт ведётся только по цифровым и знаковым разрядам

    DisplayCorrection *display;

    int Draw(int x, int y, bool test);

    // Нарисовать значок подсветки
    void HighlightSymbol(int x, int y, Color color) const;

    // Возвращает действительный индекс подсвеченного разряда (номер разряда в массиве digits)
    int IndexHighlightReal() const;

    // Количество символов, которые могут быть подсвечены
    int NumberHighligthingDigits() const;

    // Возвращает true, если в позиции pos - десятичная точка
    bool CommaInPosition(int pos) const;

    // Возвращает позицию разряда с точкой
    int PositionComma() const;

    // Возвращает true, если в позиции pos - первая значащая цифра
    bool FirstSignedDigitInPosition(int pos) const;

    // Возвращает позицию ближайшего слева цифрового знакоместа от pos. -1, если такового не имеется.
    int FindPositionLeftDigit(int pos) const;

    // Сделать активным разряд слева от текущего
    void HighlightToLeft();

    // Сделать аткивным разряд справа от текущего
    void HighlightToRight();

    // Увеличить значение в указанном разряде
    void IncreaseInPosition(int pos);

    // Уменьшить значение в указанном разряде
    void DecreaseInPosition(int pos);

    // Возвращает true, если числовое значение со знакомЫ
    bool IsSigned() const;

    // Возвращает "цену разрдяа" в позиции pos
    Value StepPosition(int pos) const;

    // Изменяет знак у параметра со знаком. Возвращает true, если pos == 0
    bool ChangeSign(Value *value, int pos);

    // Возвращает цвет, которым нужно отрисовывать символ в позиции pos. Если test == true, то функция всегда будет возвращать цвет фона
    Color CalculateColor(int pos, bool test) const;

    // Возвращает true, если нужно дополнительное смещение между разрядами
    int AdditionShiftForDigit(int pos);

    // Возвращает true, если все цифровые разряды слева равны нулю
    bool AllNumbersOfLeftIsZero(int pos) const;

    // true, если справа от pos находится точка
    bool RightDigitIzComma(int pos) const;
    
    // Возвращает true, если все цифровые разряды справа равны нулю
    bool AllNumberOfRightIsZero(int pos) const;
};


// Класс для ввода и отображения параметра
class DisplayEntering
{
public:
    
    // Обработка нажатия клавиши набора цифрового значения
    static bool OnEnteringKey(const Control &control);

    static void Draw(int x, int y, int width);

    // Обработчик нажатия кнопки "Увеличить порядок"
    static void OnButtonOrderMore();

    // Обработчик нажатия кнопки "Уменьшить порядок"
    static void OnButtonOrderLess();

    static void Init();

    // Возвращает значение, которое хранится сейчас в DisplayEntering ( c учётом порядка )
    static Value ToValue();

    // Класс для хранения вводимых данных в режиме непосредственного ввода
    class EnterBuffer
    {
    public:
        EnterBuffer() : stack(30), param(nullptr) { }
        void Prepare(ParameterDouble *parameter);
        void Push(const Key::E key);
        // Удалить последний добавленный символ
        void Pop();
        int Size() const;
        char At(const int i) const;
        // Возвращает true, если содержится десятичная точка
        bool ConsistComma() const;
        String ToString() const;
        // Возвращает true, если в буфере нет ничего, кроме, возможно, знака
        bool IsEmpty() const;

    private:
        Stack<char> stack;
        ParameterDouble *param;
    };

    class Cursor
    {
    public:
        Cursor() : timeInit(0U) { }
        void Init();
        void Draw(int x, int y);
    private:
        uint timeInit;
    };

private:

    static Cursor cursor;               // Мигающий курсор для режима непосредственного ввода

    static EnterBuffer buffer;          // Здесь будем хранить нажатые кнопки в режиме ввода

    static Order::E order;              // Текущий порядок вводимого значения

    static int DrawValue(int x, int y);

    static void DrawUnits(int x, int y, int width);

    // Попытка добавить в буфер новый символ, соотвествующий нажатой кнопке
    static void TryToAddSymbol(Key::E key);

    // Хранимое значение находится в границах, ограниченных минимальным и максимальным значениями ParameterDouble
    static bool ValueInBoundaries();
};


// Визуальное отображение Tuner
class DisplayCorrection
{
    friend class Tuner;

public:
   
    DisplayCorrection(Tuner *tuner);

    void Init();

    void Init(Value value);

    void Draw();

    bool OnControlKey(const Control &control);

    // Вывод сообщения о выходе за пределы допустимого диапазона, если необходимо
    static void ShowMessageOutRangIfNeed(Value value);

    // Рассчитывает, к какому порядку нужно привести значение для индикации (Для частоты и времени порядок плавающий, а для счётчиков и напряжения - постоянный, равный Order::One)
    static Order::E CalculateOrderForIndication();

private:

    // Ширина поля для вывода единиц измерения
    static const int WIDTH_UNITS = 25;

    Tuner *tuner;

    Indicator indicator;

    void DrawTitle(int x, int y, int width);

    void DrawUnits(int x, int y);

    // Заполнить разряды целой части параметра
    void FillDigitsIntegerPart();

    // Заполнить разряды дробной части параметра
    void FillDigitsFractPart();

    void InitDouble();

    void InitInteger();

    void DrawDouble();

    void DrawInteger();
};


// Используется для визуальной настройки параметра. Является принадлежностью каждог ParameterDouble
class Tuner
{
public:

    struct ModeTuning
    {
        enum E
        {
            Correction,     // Режим коррекциии значения параметра (ручкой)
            Entering        // Режим ввода значения
        };
    };

    Tuner(Parameter *param);

    void Init();

    void Draw();

    bool OnControlKey(const Control control);

    // Обработчик нажатия кнопки "Отмена"
    void OnButtonCancel();

    // Обработчик нажатия кнопки "Применить"
    void OnButtonApply();

    Parameter *GetParameter() { return param; }

    static bool InModeCorrection() { return (mode == ModeTuning::Correction); }

    static bool InModeEntering()   { return (mode == ModeTuning::Entering);   }

    static void SetModeEntering();

    static Tuner *Current() { return current; };

    // Возвращает true, если натсраиваемый параметр - смещение
    bool ParameterIsOffset();

    // Возвращает true, если настраиваемый параметр имеет единицы измерения "Вольт"
    bool ParameterIsVoltage();

    // Возвращает указатеь на ParameterDouble, если param имеет тип Double и nullptr в ином случае
    ParameterDouble *ReinterpretToDouble();

    ParameterInteger *ReinterpretToInteger();

private:

    Parameter *param;           // Настраиваемый параметр

    DisplayCorrection display;

    static Tuner *current;

    static ModeTuning::E mode;                         // Текущий режим настройки
};
