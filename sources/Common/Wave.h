#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Order
{
    enum E
    {
        Nano,       // нано
        Micro,      // микро
        Milli,      // милли
        One,        // единицы
        Kilo,       // кило
        Mega,       // мега
        Number
    } value;
    Order(E v = Number) : value(v)  { };
    operator uint8() const { return (uint8)value; };
    pString Name() const;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_DIGITS          5

struct Parameter
{
    enum E
    {
        Frequency,          ///< Частота
        Period,             ///< Период
        Amplitude,          ///< Амплитуда
        Offset,             ///< Смещение
        Duration,           ///< Длительность
        DutyRatio,          ///< Скважность
        Phase,              ///< Сдвиг фазы
        Delay,              ///< Задержка
        DepthModulation,    ///< Глубина амплитудной модуляции
        Polarity,           ///< Полярность
        DurationRise,       ///< Длительность нарастания
        DurationFall,       ///< Длительность спада
        DurationStady,      ///< Длительность установившего значения
        DutyFactor,         ///< Коэффициент заполнения
        Modulation,         ///< Модуляция
        Number
    } value;

    Parameter(int v = Number) : value((E)v), inNumLockMode(false) {};

    Parameter(int v, float _min, float _max, int8 pos, Order o, int8 hd = NUM_DIGITS - 1, Parameter *param = 0, int num = 0) : value((E)v), order(o), 
        hightLightDigit(hd), posComma(pos), min(_min), max(_max), inNumLockMode(false), params(param), numParams(num) { };

    float MinValue() const { return min; };

    float MaxValue() const { return max; };

    pString Name() const;
    /// Возвращает true, если параметр содержит несколько параметров
    bool IsComplexParameter() const;

    bool InNumLockMode() const { return inNumLockMode; };

    void SetNumLockMode(bool mode) { inNumLockMode = mode; };
    /// Возвращает true, если является параметром типа e
    bool Is(Parameter::E e) const { return value == e; };
    /// Возвращает true, если является непосредственно вводимым значением
    bool IsInputValue() const { return !IsComplexParameter(); };
    
    Order order;
    /// Позиция подсвеченного символа
    int8 hightLightDigit;
    /// После какой цифры воводить запятую
    int8 posComma;
    
private:

    float min;

    float max;
    /// Если true, то находимся в режиме клавиатурного ввода (кнопками 1...9)
    bool inNumLockMode;
    /// Здесь находятся дополнительные параметры в случае, если они требуются
    Parameter *params;
    /// Число дополнительных параметров. 0, если таковых не имеется
    int numParams;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Form
{
    enum E
    {
        Sine,       ///< Синус
        RampPlus,   ///< Пила+
        RampMinus,  ///< Пила-
        Meander,    ///< Меандр
        Impulse,    ///< Импульсы
        Number
    } value;

    Form(E v = Number) : value(v), currentParam(0)   { };

    Form(E v, Parameter param[Parameter::Number]);
    /// Возвращает человеческое название формы сигнала
    pString Name(Language lang) const;
    /// Возвращает ссылку на текущий параметр
    Parameter *CurrentParameter();
    /// Возвращает количество доступных параметров
    int NumParameters() const;
    /// Возвращает ссылку на i-ый параметр из массива params
    Parameter *GetParameter(int i);
    /// Установить текущим следующй параметр
    void SetNextParameter();
    /// Настраивает генератор в соответствии с установленными параметрами
    void TuneGenerator(Chan ch);
    /// Возвращает true, если тип формы сигнала соответствует e
    bool Is(Form::E e) const { return e == value; };
    /// Раскрывает страницу текущего параметра
    void OpenCurrentParameter();

private:
    /// Находит требуемый параметр. Возвращает 0, если такого параметра нет
    Parameter *FindParameter(Parameter::E p);
    /// Засыалет параметр в генератор
    void SendParameterToGenerator(Chan ch, Parameter::E p);
    Parameter params[Parameter::Number];
    /// Номер текущего параметра в массиве params
    int currentParam;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Wave
{
public:

    Wave(Chan ch, Form form[Form::Number]);

    Form forms[Form::Number];

    /// Возвращает установленную форму
    Form *GetCurrentForm();
    /// Установить текущей следующую форму
    void SetNextForm();

    int NumberOfForms() const;

    Form *GetForm(int i);
    /// Какому каналу принадлежит сигнал
    Chan channel;

private:
    /// Текущая форма сигнала - указывает на номер сигнала в массиве
    int currentForm;

public:

    class Graphics
    {
    public:
        /// Нарисовать информацию о параметрах сигнала на экране
        static void Draw(Chan ch);
        /// Позиция x области отрисовки сигнала
        static int X();
        /// Позиция y области отрисовки сигнала
        static int Y(Chan ch);
        /// Ширина области отрисовки сигнала
        static int Width();
        /// Высота области отрисовки сигнала
        static int Height();
    private:

        static void DrawUGO(Chan chan, int y0);

        static void DrawSine(int x, int y, int width, int height);

        static void DrawRampPlus(int x, int y, int width, int height);

        static void DrawRampMinus(int x, int y, int width, int height);

        static void DrawMeander(int x, int y, int width, int height);

        static void DrawImpulse(int x, int y, int width, int height);

        static void DrawParameters(Chan chan, int y0);

        static void DrawParameterValue(Chan chan, Parameter *parameter, int x, int y);
    };
};
