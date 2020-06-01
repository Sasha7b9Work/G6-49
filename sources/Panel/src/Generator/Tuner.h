#pragma once
#include "Display/Colors.h"
#include "Display/Text.h"
#include "Keyboard/Controls.h"
#include "Utils/Stack.h"


class ParameterDouble;
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
    Value StepPosition(int pos);

    // Изменяет знак у параметра со знаком. Возвращает true, если pos == 0
    bool ChangeSign(Value *value, int pos);

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
        EnterBuffer() : stack(30), param(nullptr)
        {
        }
        void Prepare(ParameterDouble *parameter);
        void Push(const Key::E key);
        // Удалить последний добавленный символ
        void Pop();
        int Size() const;
        char At(const int i) const;
        // Возвращает true, если содержится десятичная точка
        bool ConsistComma() const;
        String ToString() const;

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

    Tuner(ParameterDouble *param);

    void Init();

    void Draw();

    bool OnControlKey(const Control control);

    // Обработчик нажатия кнопки "Отмена"
    void OnButtonCancel();

    // Обработчик нажатия кнопки "Применить"
    void OnButtonApply();

    ParameterDouble *GetParameter() { return param; }

    static bool InModeCorrection() { return (mode == ModeTuning::Correction); }

    static bool InModeEntering()   { return (mode == ModeTuning::Entering);   }

    static void SetModeEntering();

    static Tuner *Current() { return current; };

private:

    ParameterDouble *param;           // Настраиваемый параметр

    DisplayCorrection display;

    static Tuner *current;

    static ModeTuning::E mode;                         // Текущий режим настройки
};
