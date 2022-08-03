#pragma once
#include "Display/Colors.h"
#include "Display/Text.h"
#include "Keyboard/Controls.h"
#include "Utils/Stack.h"


class DisplayCorrection;
struct Parameter;
struct ParameterDouble;
struct ParameterInteger;
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

    Indicator(DisplayCorrection *);

    int Draw(int x, int y, int width, const pString units) const;

    bool OnControlKey(const Control &);

    // Возвращает строку символов
    char *GetStringDigits() const;

    void InitHighlight();

    static const int MAX_NUM_DIGITS = 25;

private:

    Digit digits[MAX_NUM_DIGITS];           // Здесь находятся все символы

    int indexHighlight;                     // Индекс подсвеченного знакоместа. Счёт ведётся только по цифровым и знаковым разрядам

    DisplayCorrection *display;

    // Нарисовать значок подсветки
    void HighlightSymbol(int x, int y, const Color &) const;

    // Возвращает действительный индекс подсвеченного разряда (номер разряда в массиве digits)
    int IndexHighlightReal() const;

    // Количество символов, которые могут быть подсвечены. Не учитывается запятая
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
    void IncreaseInPositionDouble(int pos);
    void IncreaseInPositionInteger(int pos);

    // Уменьшить значение в указанном разряде
    void DecreaseInPosition(int pos);

    // Возвращает true, если числовое значение со знакомЫ
    bool IsSigned() const;

    // Возвращает "цену разрдяа" в позиции pos
    Value StepPosition(int pos) const;

    // Изменяет знак у параметра со знаком. Возвращает true, если pos == 0
    bool ChangeSign(Value *value, int pos);

    // Возвращает цвет, которым нужно отрисовывать символ в позиции pos. Если test == true, то функция всегда будет возвращать цвет фона
    Color CalculateColorDouble(int pos, bool test) const;

    Color CalculateColorInteger(int pos, bool test) const;

    // Возвращает true, если нужно дополнительное смещение между разрядами
    int AdditionShiftForDigit(int pos) const;

    // Возвращает true, если все цифровые разряды слева равны нулю
    bool AllNumbersOfLeftIsZero(int pos) const;

    // true, если справа от pos находится точка
    bool RightDigitIzComma(int pos) const;
    
    // Возвращает true, если все цифровые разряды справа равны нулю
    bool AllNumberOfRightIsZero(int pos) const;

    int DrawDouble(int x, int y, int wdidth, const pString units) const;

    int DrawDouble(int x, int y, const pString units, bool test) const;

    int DrawInteger(int x, int y, int width) const;

    int DrawInteger(int x, int y, bool test) const;
};


// Класс для ввода и отображения параметра
namespace DisplayEntering
{
    // Обработка нажатия клавиши набора цифрового значения
    bool OnEnteringKey(const Control &);

    void Draw(int x, int y, int width);

    // Обработчик нажатия кнопки "Увеличить порядок"
    void OnButtonOrderMore();

    // Обработчик нажатия кнопки "Уменьшить порядок"
    void OnButtonOrderLess();

    void Init();

    // Возвращает значение, которое хранится сейчас в DisplayEntering ( c учётом порядка )
    Value ToValue();

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
        // Возвращает количетсво цифр после десятичной точки
        int NumDigitsAfterComma() const;
        // Возвращает позицию десятичной точки
        int PosComma() const;

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

    bool OnControlKey(const Control &);

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

    // Заполнить разряды целой части параметра
    void FillDigitsIntegerPartForDouble();

    // Заполнить разряды дробной части параметра
    void FillDigitsFractPartForDouble();

    void FillDigitsForInteger();

    void InitDouble();

    void InitInteger();

    void DrawDouble(int x, int y);

    void DrawInteger(int x, int y);
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

    bool OnControlKey(const Control &);

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
    bool ParameterIsNotOrdered();

    static bool ParameterIsDouble() { return Current()->ReinterpretToDouble() != nullptr; };

    // Возвращает указатеь на ParameterDouble, если param имеет тип Double и nullptr в ином случае
    ParameterDouble *ReinterpretToDouble();

    ParameterInteger *ReinterpretToInteger();

private:

    Parameter *param;           // Настраиваемый параметр

    DisplayCorrection display;

    static Tuner *current;

    static ModeTuning::E mode;                         // Текущий режим настройки
};
