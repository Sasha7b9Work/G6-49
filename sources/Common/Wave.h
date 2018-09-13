#pragma once
#include "defines.h"
#include <string.h>


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

class Form;

class Parameter
{
public:
    friend class Form;

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

    Parameter(int v, float _min, float _max, pString buf, int8 pos, Order o, Parameter *param = 0, 
        int num = 0, int8 hd = NUM_DIGITS - 1, char s = ' ') :
        value((E)v), order(o), hightLightDigit(hd), posComma(pos), sign(s), min(_min), max(_max), inNumLockMode(false), params(param), numParams(num),
        parent(0)
    {
        strcpy(buffer, buf);
    };

    float MinValue() const { return min; };

    float MaxValue() const { return max; };

    float GetValue()  const;

    char *GetStringValue() const;

    pString Name() const;
    /// Возвращает true, если параметр содержит несколько параметров
    bool IsComplexParameter() const;

    bool InNumLockMode() const { return inNumLockMode; };

    void SetNumLockMode(bool mode) { inNumLockMode = mode; };
    /// Возвращает true, если является параметром типа e
    bool Is(Parameter::E e) const { return value == e; };
    /// Возвращает true, если является непосредственно вводимым значением
    bool IsInputValue() const { return !IsComplexParameter(); };
    /// Возвращает указатель на форму, параметром которой является
    Form *GetForm() { return form; };
    /// Возвращает адрес родительского параметра
    Parameter *GetParent() { return parent; };
   
    Order order;
    /// Позиция подсвеченного символа
    int8 hightLightDigit;
    /// После какой цифры воводить запятую
    int8 posComma;
    /// В этом буфере хранится знак числа, а затем цифры с точкой
    char buffer[NUM_DIGITS + 1];
    /// Знак числа. ' ' в случае, если число знака не имеет - строго положительное
    char sign;
private:

    float min;

    float max;
    /// Если true, то находимся в режиме клавиатурного ввода (кнопками 1...9)
    bool inNumLockMode;
    /// Здесь находятся дополнительные параметры в случае, если они требуются
    Parameter *params;
    /// Число дополнительных параметров. 0, если таковых не имеется
    int numParams;
    /// Указатель на фрорму, которой принадлежит параметр
    Form *form;
    /// Если этот параметр вложенный, то здесь адрес родителя
    Parameter *parent;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Form
{
public:
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

    Form(E v, Parameter *param, int numParams);
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
    /// Закрывает открытый параметр, если таковой имеется и возвращает true в этом случае
    bool CloseOpenedParameter();

private:
    /// Находит требуемый параметр. Возвращает 0, если такого параметра нет
    Parameter *FindParameter(Parameter::E p);
    /// Засыалет параметр в генератор
    void SendParameterToGenerator(Chan ch, Parameter::E p);
    /// Здесь хранятся параметры
    Parameter *params;
    /// Сколько всего параметров
    int numParams;
    /// Номер текущего параметра в массиве params
    int currentParam;
    /// Здесь сохраняется указатель на основные параметры в случае раскрытия сложного параметра
    Parameter *oldParams;
    /// Относится к oldParams
    int oldNumParams;
    /// Относится к oldParams
    int oldCurrentParams;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Wave
{
public:

    Wave(Chan ch, Form *forms, int numForms);
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
    /// Список форм, которые могут быть назначены
    Form *forms;
    /// Количество возможных форм
    int numForms;

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

        static void DrawParameterValue(Parameter *parameter, int x, int y);
    };
};
